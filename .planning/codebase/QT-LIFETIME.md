# Qt Object Lifetime & Ownership Conventions (VIEW / BALLView)

**Status:** v1.7.x-26 deliverable — ownership audit + conventions.
**Scope:** `source/VIEW/**`, `source/APPLICATIONS/BALLVIEW/**`, `include/BALL/VIEW/**`.
**Why this exists:** The 1.6→1.7 UI redesign introduced a cluster of
lifetime bugs (UFG-30, UFG-32, the inspector dangling-section crash, the
welcome-screen / migration-modal hangs). Every one of them was a
*lifetime / ownership* defect, not a logic defect: a raw pointer outliving
its object, a destructor touching an already-destroyed child, or a nested
event loop entered from the wrong place. This document codifies the rules
that prevent the whole class.

This is the lifetime companion to `CONVENTIONS.md` (naming/style) and the
architecture rules in `.planning/v1.7.x-ARCHITECTURE-REVIEW.md` (§7
command/transaction contract, Qt-lifetime as its own concern).

---

## 1. The one rule: every QObject has exactly one owner

A `QObject*` is owned by **either**:

- **(A) its Qt parent** — passed to the constructor (`new QLabel(this)`) or
  set via `setParent()` / `layout->addWidget()`. Qt deletes it when the
  parent dies. This is the default and preferred path.
- **(B) a C++ owner** — a `boost::shared_ptr`, a `std::unique_ptr`, a
  container, or an explicit `delete` in a destructor. Used when the object
  must outlive/underlive its visual parent, or has no widget parent
  (e.g. `RenderSetup` held in `renderers_`).

**Never both.** A QObject that has a Qt parent *and* a `shared_ptr`/explicit
`delete` is a double-free waiting to happen. If you take a `shared_ptr` to a
QObject, give it `nullptr` parent (or a parent guaranteed to outlive the
shared_ptr, with the shared_ptr never being the one to free it).

**Raw pointers are observers, never owners** (with the single legacy
exception of dialog-local `new`/`delete` pairs). A raw `Foo* foo_` member is
a *non-owning reference* to something owned elsewhere. Its only obligations:
do not `delete` it, and do not dereference it after the owner may have freed
it (see §3).

---

## 2. Parent-child destruction order — the UFG-30 / UFG-32 trap

When a `QObject` is destroyed, Qt destroys its children **in unspecified
order relative to the parent's own destructor body**. Concretely:

> A child widget can be destroyed **before** the destructor of a sibling
> QObject (or a non-widget QObject parented to the same window) runs.

This produced two shipped crashes:

- **UFG-30** (`workspaceManager.C:351`): `~WorkspaceStatusLabel` called
  `label_->removeEventFilter(this)`. `label_` (a child `QLabel` of the same
  `QMainWindow`) was already torn down → `KERN_INVALID_ADDRESS` inside
  `QObject::removeEventFilter`.
- **UFG-32** (`inspectorBody.C:184`): `clearSections()` used to
  `w->deleteLater()` on section widgets that `InspectorView` *also* held as
  raw-pointer members and reused → dangling-pointer crash at
  `QLayout::addChildWidget`.

**Rules:**

1. **Do NOT call `removeEventFilter` on a child in a destructor.** Qt
   auto-removes all event filters when the watched object is destroyed.
   The explicit cleanup is both unnecessary and crash-prone. (See the
   documented `~WorkspaceStatusLabel` for the canonical comment.) If you
   genuinely must remove a filter early, the watched pointer must be a
   `QPointer` and you must null-check it.
2. **A widget is owned by exactly one place.** If `InspectorView` holds
   `model_section_` as a reused raw member, then `InspectorBody` must NOT
   `deleteLater()` it — it only detaches it from the layout. Decide the
   owner once; everyone else observes.
3. **In a destructor, touch only what you own.** Reading/writing sibling or
   child state in a dtor assumes a destruction order Qt does not guarantee.

---

## 3. Cross-owner references → `QPointer`, never raw

When object A holds a pointer to object B that A does **not** own, and B can
be destroyed independently of A, the pointer must be a `QPointer<B>`. A
`QPointer` auto-nulls when B is destroyed, so `if (ptr)` is a valid liveness
check.

Current `QPointer` users (correct): `commandRegistry.{h,C}`,
`workspaceManager.C`.

**Candidates that should migrate to `QPointer`** (raw cross-owner refs that
survive only because of incidental ordering):

- `WorkspaceStatusLabel::label_` — explicitly called out in the UFG-30
  comment as the right fix ("once it switches `label_` to `QPointer<QLabel>`
  ... re-introduce a guarded `removeEventFilter`").
- Any controller→widget or widget→controller back-reference where the two
  have different owners. Audit during v1.7.x-24 (Controller cut-over): a
  Controller outlives the dialog/section that drives it, so a Controller
  must never hold a raw `Section*`; a Section holding a raw `Controller*` is
  fine **only** if the Controller is guaranteed to outlive the Section
  (document the guarantee at the member declaration).

**Rule:** if you cannot point at the line that guarantees the referent
outlives the reference, use `QPointer` and null-check.

---

## 4. `deleteLater()` — null the raw pointer in the same statement block

`deleteLater()` posts a deferred-delete event; the object dies the next time
the event loop spins. Between the call and the next loop turn the raw
pointer is **live but doomed**. Any code that dereferences it in that window
(another slot, a re-entrant call) touches a soon-to-be-freed object.

**Rule:** immediately after `x->deleteLater()`, set `x = nullptr` (when `x`
is a retained member), unless the very next statements reassign `x` to a
fresh object with no intervening early-return or event-loop spin.

Audit results:

| Site | Verdict |
|------|---------|
| `inspectorBody.C:146,171` (`emptyState`) | ✅ nulls immediately after |
| `welcomeScreen.C:471` (`onWhatsNewDismiss_`) | ✅ nulls immediately after |
| `welcomeScreen.C:392` (`showWhatsNew`) | ⚠️ **latent**: `deleteLater()` then an early `return` if the markdown file fails to open, leaving `whats_new_card_` pointing at a doomed object; reassigned to `new QFrame` only on the success path. Low severity (missing-file edge), but fix by moving the `deleteLater()`+null below the early return, or null immediately. |
| `propertyEditor.C:116,329` | local/transient editors, owner-confined — OK |
| download dialogs (`current_reply_`, `network_manager_`, `progress_bar_`) | reassigned on next use — OK |

**Action for v1.7.x:** fix the `welcomeScreen.C:392` latent case (1-line
move). Everything else conforms.

---

## 5. Event filters

`installEventFilter` does **not** transfer ownership and does **not** need a
matching `removeEventFilter` for correctness — Qt removes the filter
automatically when **either** the watched object **or** the filter object is
destroyed.

Audit (all SAFE under the auto-removal rule):

- `qApp->installEventFilter(this)` — `testFramework.C`, `helpViewer.C`,
  `logView.C`, `mainframe.C`. App-global filters; `qApp` outlives all of
  them. `testFramework`/`helpViewer` correctly pair an explicit
  `removeEventFilter` because they uninstall *before* their own destruction
  (early uninstall is fine; it's the *destructor* removal of a child that's
  the trap).
- `line_->installEventFilter(this)` (`commandPalette.C:284`) — `line_` is a
  child of `this`; dtor is `= default` (no manual remove) — correct.
- `pro->installEventFilter(this)` (`peptideDialog.C:57`),
  `editor->installEventFilter(this)` (`pythonSettings.C:50`) — filter object
  outlives the watched widget; auto-removed — correct.
- `surface_obj->installEventFilter(this)` (`scene.C:454`) — Scene watches the
  GL widget it effectively owns via `renderers_`; auto-removed — correct.
- `new_widget->installEventFilter(this)` (`scene.C:2549`, `:2575`) — both in
  **dead/disabled code** (`:2549` is preceded by an unconditional `return`;
  `:2575` is `BALL_HAS_RTFACT`-gated, off). No live risk.

**Rule:** rely on auto-removal. Add an explicit `removeEventFilter` ONLY for
early uninstall (object still alive on both ends), never in a destructor on a
child/sibling. If early removal in a dtor is truly required, the watched
pointer must be `QPointer`.

---

## 6. GL-context ownership (`makeCurrent` / `doneCurrent`)

The OpenGL context is owned by the `QOpenGLWidget` (`GLRenderWindow`). Any
GL call must run with that context current. Two failure modes seen in 1.7:

1. GL calls off the paint path without `makeCurrent()` → silent no-op or
   crash on Metal-backed GL.
2. `Scene::updateGL()` calling `paintGL()` directly (documented hack/TODO in
   `scene.C`) — bypasses Qt's context bookkeeping.

**Rules:**

- Every standalone GL mutation (display-list build, VBO upload, texture
  alloc) outside `paintGL()` must bracket with `makeCurrent()` … work …
  (Qt restores on the next `paintGL`; explicit `doneCurrent()` only when
  handing the context to another thread).
- Treat the context as owned by the widget: do not cache GL handles across a
  context recreation (resize / reparent destroys+recreates the context).
- The `updateGL()→paintGL()` direct call is a known debt item — route repaint
  requests through `update()` (schedules a paint with a current context), not
  a direct `paintGL()`. Tracked for the Renderer/RenderSurface cut-over.

---

## 7. Nested event loops are a hang risk in headless / startup paths

`QDialog::exec()`, `QMessageBox::information/question`, and any modal
`exec()` spin a **nested event loop**. On a headless or automated startup
path that loop never gets a user response → hang. This caused the smoke-test
hang (migration modal) fixed by the `BALLVIEW_NO_WELCOME` bypass
(v1.7.x-15), and is the same hazard behind the reset-confirm dialogs added
in v1.7.x-18.

**Rules:**

- Any modal `exec()` reachable during construction/startup must be guarded
  by an env/headless check (`qEnvironmentVariableIsSet("BALLVIEW_NO_WELCOME")`
  is the established gate) **before** the loop is entered — return early,
  don't merely skip the body.
- Prefer non-modal `open()` + signal callbacks over `exec()` for anything not
  strictly blocking a user decision.
- Never enter a nested loop from inside a paint, a timer fire, or a
  ConnectionObject message dispatch — re-entrancy into the message bus is the
  v1.7.x-24 re-entrancy-guard concern.

---

## 8. Checklist for new VIEW code (PR-time)

- [ ] Every `new QObject` either has a parent **or** a documented C++ owner —
      not both.
- [ ] No raw-pointer member is `delete`d/`deleteLater`d by more than one
      place. Owner is singular and documented.
- [ ] After `deleteLater()` on a retained member, the member is nulled before
      any possible reuse or early return.
- [ ] Cross-owner pointer with independent lifetimes → `QPointer` + null
      check.
- [ ] No `removeEventFilter` on a child/sibling in a destructor.
- [ ] GL calls outside `paintGL()` bracket `makeCurrent()`; repaint via
      `update()`, not direct `paintGL()`.
- [ ] No modal `exec()` reachable on the startup path without a headless
      bypass guard.

---

## 9. Highest-risk remaining sites (for v1.7.x follow-up)

1. **`welcomeScreen.C:392`** — latent dangling `whats_new_card_` on the
   markdown-open-failure early-return path (§4). 1-line fix.
2. **`WorkspaceStatusLabel::label_`** — should become `QPointer<QLabel>` to
   make the UFG-30 contract enforceable rather than commented (§3).
3. **`scene.C` `updateGL()→paintGL()` direct call** — context-bookkeeping
   debt; resolve in the Renderer/RenderSurface cut-over (§6).
4. **Controller↔Section back-references (v1.7.x-24)** — when Controllers
   become command handlers, audit every Controller/Section pointer pair for
   the outlives-guarantee; default to `QPointer` on the shorter-lived side
   (§3).
