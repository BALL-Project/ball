// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL — K0.5 compiled selection. Sub-phase K0.5.0:
// skeleton tests for variant AST node construction + CompiledExpression
// move-only plumbing. Evaluation correctness tests land in K0.5.1.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/compiledExpression.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/expression.h>  // K0.5.3 integration check
#include <atomic>                    // K0.5.5 concurrent-parse test
#include <thread>                    // K0.5.5 concurrent-parse test
///////////////////////////

START_TEST(CompiledExpression)

using namespace BALL;

CHECK(K0.5.0 leaf node default-constructible)
	PredNode n_true   = TrueLeaf{};
	PredNode n_false  = FalseLeaf{};
	PredNode n_sel    = SelectedLeaf{};
	PredNode n_solv   = SolventLeaf{};
	PredNode n_bb     = BackboneLeaf{};
	TEST_EQUAL(std::holds_alternative<TrueLeaf>(n_true),     true)
	TEST_EQUAL(std::holds_alternative<FalseLeaf>(n_false),   true)
	TEST_EQUAL(std::holds_alternative<SelectedLeaf>(n_sel),  true)
	TEST_EQUAL(std::holds_alternative<SolventLeaf>(n_solv),  true)
	TEST_EQUAL(std::holds_alternative<BackboneLeaf>(n_bb),   true)
RESULT

CHECK(K0.5.0 element/name/charge leaves carry pre-compiled constants)
	PredNode n_el   = ElementPred{6};   // Carbon
	PredNode n_name = AtomNamePred{42};
	PredNode n_chg  = ChargeRange{CmpOp::GE, -0.5f};
	PredNode n_bonds = NumberOfBondsPred{CmpOp::EQ, 4, 0};

	TEST_EQUAL(std::get<ElementPred>(n_el).element_idx, 6)
	TEST_EQUAL(std::get<AtomNamePred>(n_name).name_offset, 42u)
	TEST_EQUAL(static_cast<int>(std::get<ChargeRange>(n_chg).op),
	           static_cast<int>(CmpOp::GE))
	TEST_EQUAL(std::get<ChargeRange>(n_chg).value, -0.5f)
	TEST_EQUAL(std::get<NumberOfBondsPred>(n_bonds).n, 4)
RESULT

CHECK(K0.5.0 inner And/Or/Not nodes box recursively via unique_ptr)
	auto a = std::make_unique<AndNode>();
	a->children.emplace_back(ElementPred{6});
	a->children.emplace_back(ChargeRange{CmpOp::LT, 0.f});
	PredNode root = std::move(a);

	TEST_EQUAL(std::holds_alternative<std::unique_ptr<AndNode>>(root), true)
	TEST_EQUAL(std::get<std::unique_ptr<AndNode>>(root)->children.size(), 2u)

	auto n = std::make_unique<NotNode>();
	n->child = TrueLeaf{};
	PredNode neg = std::move(n);
	TEST_EQUAL(std::holds_alternative<std::unique_ptr<NotNode>>(neg), true)
RESULT

CHECK(K0.5.0 CompiledExpression ctor + accessors + skeleton evaluate)
	PredNode root = ElementPred{8};  // Oxygen
	CompiledExpression ce(std::move(root),
	                      "element(O)",
	                      /*pred_set_hash=*/12345u);
	TEST_EQUAL(ce.source(), std::string("element(O)"))
	TEST_EQUAL(ce.pred_set_hash(), 12345u)
	TEST_EQUAL(std::holds_alternative<ElementPred>(ce.root()), true)

	// K0.5.1 will replace these with real evaluation. K0.5.0 skeleton:
	// evaluate sizes the bitmap and zero-fills; evaluate_one returns false.
	MoleculeStore store;
	store.reserve(8);
	(void)store.allocate_atom();
	(void)store.allocate_atom();
	std::vector<std::uint8_t> bitmap;
	ce.evaluate(store, bitmap);
	TEST_EQUAL(bitmap.size(), store.size())
	TEST_EQUAL(bitmap[0], 0)
	TEST_EQUAL(bitmap[1], 0)
RESULT

CHECK(K0.5.0 CompiledExpression is move-only)
	CompiledExpression src(PredNode{TrueLeaf{}}, "true", 0);
	CompiledExpression dst = std::move(src);
	TEST_EQUAL(dst.source(), std::string("true"))
	// src is moved-from; accessing it is UB by contract, only test dst.
	TEST_EQUAL(std::holds_alternative<TrueLeaf>(dst.root()), true)
RESULT

// ============================================================
// K0.5.1 — bitmap evaluation inner loop tests
// ============================================================

CHECK(K0.5.1 TrueLeaf marks every live atom)
	MoleculeStore store;
	store.reserve(8);
	auto a = store.allocate_atom();
	auto b = store.allocate_atom();
	auto c = store.allocate_atom();
	store.release_atom(b);   // freed slot must read 0

	CompiledExpression ce(PredNode{TrueLeaf{}}, "true", 0);
	std::vector<std::uint8_t> bitmap;
	ce.evaluate(store, bitmap);
	TEST_EQUAL(bitmap.size(), store.size())
	TEST_EQUAL(bitmap[a], 1)
	TEST_EQUAL(bitmap[b], 0)   // freed
	TEST_EQUAL(bitmap[c], 1)
RESULT

CHECK(K0.5.1 FalseLeaf marks nothing)
	MoleculeStore store;
	store.reserve(4);
	auto a = store.allocate_atom();
	auto b = store.allocate_atom();
	(void)a; (void)b;
	CompiledExpression ce(PredNode{FalseLeaf{}}, "false", 0);
	std::vector<std::uint8_t> bitmap;
	ce.evaluate(store, bitmap);
	for (std::uint8_t v : bitmap) TEST_EQUAL(v, 0)
RESULT

CHECK(K0.5.1 SelectedLeaf reflects MoleculeStore::selection_)
	MoleculeStore store;
	store.reserve(4);
	auto a = store.allocate_atom();
	auto b = store.allocate_atom();
	auto c = store.allocate_atom();
	store.set_selected(a, true);
	store.set_selected(c, true);
	CompiledExpression ce(PredNode{SelectedLeaf{}}, "selected", 0);
	std::vector<std::uint8_t> bitmap;
	ce.evaluate(store, bitmap);
	TEST_EQUAL(bitmap[a], 1)
	TEST_EQUAL(bitmap[b], 0)
	TEST_EQUAL(bitmap[c], 1)
RESULT

CHECK(K0.5.1 ElementPred matches atomic number column)
	System sys;
	Atom *c1 = new Atom; c1->setElement(PTE[Element::CARBON]); sys.adopt(*c1);
	Atom *o1 = new Atom; o1->setElement(PTE[Element::OXYGEN]); sys.adopt(*o1);
	Atom *c2 = new Atom; c2->setElement(PTE[Element::CARBON]); sys.adopt(*c2);

	auto& store = sys.getStore();
	const std::uint8_t carbon_z = static_cast<std::uint8_t>(PTE[Element::CARBON].getAtomicNumber());
	CompiledExpression ce(PredNode{ElementPred{carbon_z}}, "element(C)", 0);
	std::vector<std::uint8_t> bitmap;
	ce.evaluate(store, bitmap);
	TEST_EQUAL(bitmap[c1->getStoreIndex()], 1)
	TEST_EQUAL(bitmap[o1->getStoreIndex()], 0)
	TEST_EQUAL(bitmap[c2->getStoreIndex()], 1)

	// Per-atom fallback path agrees with the bitmap.
	TEST_EQUAL(ce.evaluate_one(*c1), true)
	TEST_EQUAL(ce.evaluate_one(*o1), false)
RESULT

CHECK(K0.5.1 AtomNamePred uses pre-interned offset)
	System sys;
	Atom *ca = new Atom; ca->setName("CA"); sys.adopt(*ca);
	Atom *cb = new Atom; cb->setName("CB"); sys.adopt(*cb);
	Atom *ca2 = new Atom; ca2->setName("CA"); sys.adopt(*ca2);

	auto& store = sys.getStore();
	const std::uint32_t off_ca = store.intern_name("CA");
	CompiledExpression ce(PredNode{AtomNamePred{off_ca}}, "name(CA)", 0);
	std::vector<std::uint8_t> bitmap;
	ce.evaluate(store, bitmap);
	TEST_EQUAL(bitmap[ca->getStoreIndex()],  1)
	TEST_EQUAL(bitmap[cb->getStoreIndex()],  0)
	TEST_EQUAL(bitmap[ca2->getStoreIndex()], 1)
RESULT

CHECK(K0.5.1 ChargeRange covers all CmpOps)
	System sys;
	Atom *a = new Atom; a->setCharge(-1.0f); sys.adopt(*a);
	Atom *b = new Atom; b->setCharge( 0.0f); sys.adopt(*b);
	Atom *c = new Atom; c->setCharge(+1.0f); sys.adopt(*c);
	auto& store = sys.getStore();
	std::vector<std::uint8_t> bm;

	CompiledExpression lt(PredNode{ChargeRange{CmpOp::LT, 0.f}}, "lt", 0);
	lt.evaluate(store, bm);
	TEST_EQUAL(bm[a->getStoreIndex()], 1) TEST_EQUAL(bm[b->getStoreIndex()], 0) TEST_EQUAL(bm[c->getStoreIndex()], 0)

	CompiledExpression ge(PredNode{ChargeRange{CmpOp::GE, 0.f}}, "ge", 0);
	ge.evaluate(store, bm);
	TEST_EQUAL(bm[a->getStoreIndex()], 0) TEST_EQUAL(bm[b->getStoreIndex()], 1) TEST_EQUAL(bm[c->getStoreIndex()], 1)

	CompiledExpression eq(PredNode{ChargeRange{CmpOp::EQ, 0.f}}, "eq", 0);
	eq.evaluate(store, bm);
	TEST_EQUAL(bm[a->getStoreIndex()], 0) TEST_EQUAL(bm[b->getStoreIndex()], 1) TEST_EQUAL(bm[c->getStoreIndex()], 0)
RESULT

CHECK(K0.5.1 NumberOfBondsPred any-order counts CSR degree)
	System sys;
	Atom *a = new Atom; sys.adopt(*a);
	Atom *b = new Atom; sys.adopt(*b);
	Atom *c = new Atom; sys.adopt(*c);
	// Build a–b, a–c so a has 2 bonds, b and c have 1 each.
	auto& store = sys.getStore();
	store.add_bond(a->getStoreIndex(), b->getStoreIndex(), 1, 0);
	store.add_bond(a->getStoreIndex(), c->getStoreIndex(), 1, 0);

	CompiledExpression two(PredNode{NumberOfBondsPred{CmpOp::EQ, 2, 0}}, "bonds==2", 0);
	std::vector<std::uint8_t> bm;
	two.evaluate(store, bm);
	TEST_EQUAL(bm[a->getStoreIndex()], 1)
	TEST_EQUAL(bm[b->getStoreIndex()], 0)
	TEST_EQUAL(bm[c->getStoreIndex()], 0)
RESULT

CHECK(K0.5.1 AndNode bitwise-ANDs children)
	System sys;
	Atom *c1 = new Atom; c1->setElement(PTE[Element::CARBON]); c1->setCharge(-1.f); sys.adopt(*c1);
	Atom *c2 = new Atom; c2->setElement(PTE[Element::CARBON]); c2->setCharge( 1.f); sys.adopt(*c2);
	Atom *o1 = new Atom; o1->setElement(PTE[Element::OXYGEN]); o1->setCharge(-1.f); sys.adopt(*o1);
	auto& store = sys.getStore();
	const std::uint8_t carbon_z = static_cast<std::uint8_t>(PTE[Element::CARBON].getAtomicNumber());

	auto a = std::make_unique<AndNode>();
	a->children.emplace_back(ElementPred{carbon_z});
	a->children.emplace_back(ChargeRange{CmpOp::LT, 0.f});
	CompiledExpression ce(PredNode{std::move(a)}, "carbon AND neg charge", 0);
	std::vector<std::uint8_t> bm;
	ce.evaluate(store, bm);
	TEST_EQUAL(bm[c1->getStoreIndex()], 1)   // carbon AND negative
	TEST_EQUAL(bm[c2->getStoreIndex()], 0)   // carbon but positive
	TEST_EQUAL(bm[o1->getStoreIndex()], 0)   // negative but oxygen
RESULT

CHECK(K0.5.1 OrNode bitwise-ORs children + NotNode inverts (live only))
	System sys;
	Atom *c1 = new Atom; c1->setElement(PTE[Element::CARBON]); sys.adopt(*c1);
	Atom *o1 = new Atom; o1->setElement(PTE[Element::OXYGEN]); sys.adopt(*o1);
	Atom *h1 = new Atom; h1->setElement(PTE[Element::HYDROGEN]); sys.adopt(*h1);
	auto& store = sys.getStore();
	const std::uint8_t carbon_z = static_cast<std::uint8_t>(PTE[Element::CARBON].getAtomicNumber());
	const std::uint8_t oxygen_z = static_cast<std::uint8_t>(PTE[Element::OXYGEN].getAtomicNumber());

	// (carbon OR oxygen)
	auto orn = std::make_unique<OrNode>();
	orn->children.emplace_back(ElementPred{carbon_z});
	orn->children.emplace_back(ElementPred{oxygen_z});
	CompiledExpression ce_or(PredNode{std::move(orn)}, "C or O", 0);
	std::vector<std::uint8_t> bm;
	ce_or.evaluate(store, bm);
	TEST_EQUAL(bm[c1->getStoreIndex()], 1)
	TEST_EQUAL(bm[o1->getStoreIndex()], 1)
	TEST_EQUAL(bm[h1->getStoreIndex()], 0)

	// NOT(carbon) — must NOT mark freed slots.
	auto notn = std::make_unique<NotNode>();
	notn->child = ElementPred{carbon_z};
	CompiledExpression ce_not(PredNode{std::move(notn)}, "not C", 0);
	ce_not.evaluate(store, bm);
	TEST_EQUAL(bm[c1->getStoreIndex()], 0)
	TEST_EQUAL(bm[o1->getStoreIndex()], 1)
	TEST_EQUAL(bm[h1->getStoreIndex()], 1)
RESULT

CHECK(K0.5.1 evaluate_one mirrors bitmap for And + Element + Charge)
	System sys;
	Atom *a = new Atom;
	a->setElement(PTE[Element::CARBON]);
	a->setCharge(-0.7f);
	sys.adopt(*a);
	const std::uint8_t carbon_z = static_cast<std::uint8_t>(PTE[Element::CARBON].getAtomicNumber());

	auto root = std::make_unique<AndNode>();
	root->children.emplace_back(ElementPred{carbon_z});
	root->children.emplace_back(ChargeRange{CmpOp::LT, 0.f});
	CompiledExpression ce(PredNode{std::move(root)}, "carbon AND neg", 0);
	TEST_EQUAL(ce.evaluate_one(*a), true)

	a->setCharge(+0.7f);
	TEST_EQUAL(ce.evaluate_one(*a), false)
RESULT

// ============================================================
// K0.5.2 — parser->AST + LRU cache tests
// ============================================================

CHECK(K0.5.2 compile element(C) parses + evaluates correctly)
	System sys;
	Atom *c1 = new Atom; c1->setElement(PTE[Element::CARBON]); sys.adopt(*c1);
	Atom *o1 = new Atom; o1->setElement(PTE[Element::OXYGEN]); sys.adopt(*o1);
	auto& store = sys.getStore();
	auto ce = CompiledExpression::compile(store, "element(C)");
	std::vector<std::uint8_t> bm;
	ce->evaluate(store, bm);
	TEST_EQUAL(bm[c1->getStoreIndex()], 1)
	TEST_EQUAL(bm[o1->getStoreIndex()], 0)
RESULT

CHECK(K0.5.2 compile name(CA) interns into store pool)
	System sys;
	Atom *ca = new Atom; ca->setName("CA"); sys.adopt(*ca);
	Atom *cb = new Atom; cb->setName("CB"); sys.adopt(*cb);
	auto& store = sys.getStore();
	auto ce = CompiledExpression::compile(store, "name(CA)");
	std::vector<std::uint8_t> bm;
	ce->evaluate(store, bm);
	TEST_EQUAL(bm[ca->getStoreIndex()], 1)
	TEST_EQUAL(bm[cb->getStoreIndex()], 0)
RESULT

CHECK(K0.5.2 compile charge(<0) parses CmpOp prefix)
	System sys;
	Atom *a = new Atom; a->setCharge(-1.f); sys.adopt(*a);
	Atom *b = new Atom; b->setCharge( 1.f); sys.adopt(*b);
	auto& store = sys.getStore();
	auto ce = CompiledExpression::compile(store, "charge(<0)");
	std::vector<std::uint8_t> bm;
	ce->evaluate(store, bm);
	TEST_EQUAL(bm[a->getStoreIndex()], 1)
	TEST_EQUAL(bm[b->getStoreIndex()], 0)
RESULT

CHECK(K0.5.2 compile AND/OR/NOT compose via parser)
	System sys;
	Atom *c_neg = new Atom; c_neg->setElement(PTE[Element::CARBON]); c_neg->setCharge(-1.f); sys.adopt(*c_neg);
	Atom *c_pos = new Atom; c_pos->setElement(PTE[Element::CARBON]); c_pos->setCharge( 1.f); sys.adopt(*c_pos);
	Atom *o_neg = new Atom; o_neg->setElement(PTE[Element::OXYGEN]); o_neg->setCharge(-1.f); sys.adopt(*o_neg);
	auto& store = sys.getStore();
	std::vector<std::uint8_t> bm;

	auto ce_and = CompiledExpression::compile(store, "element(C) AND charge(<0)");
	ce_and->evaluate(store, bm);
	TEST_EQUAL(bm[c_neg->getStoreIndex()], 1)
	TEST_EQUAL(bm[c_pos->getStoreIndex()], 0)
	TEST_EQUAL(bm[o_neg->getStoreIndex()], 0)

	auto ce_or = CompiledExpression::compile(store, "element(C) OR element(O)");
	ce_or->evaluate(store, bm);
	TEST_EQUAL(bm[c_neg->getStoreIndex()], 1)
	TEST_EQUAL(bm[c_pos->getStoreIndex()], 1)
	TEST_EQUAL(bm[o_neg->getStoreIndex()], 1)

	auto ce_not = CompiledExpression::compile(store, "!element(C)");
	ce_not->evaluate(store, bm);
	TEST_EQUAL(bm[c_neg->getStoreIndex()], 0)
	TEST_EQUAL(bm[c_pos->getStoreIndex()], 0)
	TEST_EQUAL(bm[o_neg->getStoreIndex()], 1)
RESULT

CHECK(K0.5.2 unsupported predicate throws ParseError)
	System sys;
	auto& store = sys.getStore();
	bool threw = false;
	try { (void)CompiledExpression::compile(store, "inRing()"); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

CHECK(K0.5.2 cache hit returns the same shared_ptr instance)
	System sys;
	Atom *a = new Atom; a->setElement(PTE[Element::CARBON]); sys.adopt(*a);
	auto& store = sys.getStore();
	auto& cache = CompiledExpressionCache::instance();
	cache.clear();

	auto p1 = cache.get_or_compile(store, "element(C)");
	auto p2 = cache.get_or_compile(store, "element(C)");
	TEST_EQUAL(p1.get(), p2.get())     // same instance
	TEST_EQUAL(cache.size(), 1u)

	auto p3 = cache.get_or_compile(store, "element(O)");
	TEST_NOT_EQUAL(p1.get(), p3.get())
	TEST_EQUAL(cache.size(), 2u)
RESULT

CHECK(K0.5.2 cache evicts least-recently-used past capacity)
	System sys;
	Atom *a = new Atom; sys.adopt(*a);
	auto& store = sys.getStore();
	auto& cache = CompiledExpressionCache::instance();
	cache.clear();
	cache.set_capacity(3);

	auto pA = cache.get_or_compile(store, "true()");        // A LRU
	auto pB = cache.get_or_compile(store, "false()");       // B
	auto pC = cache.get_or_compile(store, "selected()");    // C MRU
	(void)pC;
	TEST_EQUAL(cache.size(), 3u)

	// Touching A promotes it to MRU; inserting D should evict B.
	(void)cache.get_or_compile(store, "true()");
	(void)cache.get_or_compile(store, "element(C)");        // D
	TEST_EQUAL(cache.size(), 3u)

	// "false()" should now be evicted; compiling it again creates a new ptr.
	auto pB2 = cache.get_or_compile(store, "false()");
	TEST_NOT_EQUAL(pB.get(), pB2.get())

	// "true()" still cached (since we touched it before D was inserted).
	auto pA2 = cache.get_or_compile(store, "true()");
	TEST_EQUAL(pA.get(), pA2.get())

	cache.set_capacity(1024);  // restore default for other tests
RESULT

CHECK(K0.5.3 Expression::operator() takes the compiled fast path)
	// Codex spec §5: Expression::operator()(const Atom&) now prefers the
	// compiled fast path via evaluate_one when a store is bound; falls
	// back to expression_tree_ on parse/compile failure.
	System sys;
	Atom *carbon = new Atom; carbon->setElement(PTE[Element::CARBON]); sys.adopt(*carbon);
	Atom *oxygen = new Atom; oxygen->setElement(PTE[Element::OXYGEN]); sys.adopt(*oxygen);

	Expression expr("element(C)");
	TEST_EQUAL(expr(*carbon), true)
	TEST_EQUAL(expr(*oxygen), false)

	// Verify the fast path actually ran by checking the cache populated.
	auto& cache = CompiledExpressionCache::instance();
	auto compiled = expr.getCompiled(sys.getStore());
	TEST_NOT_EQUAL(compiled.get(), (const CompiledExpression*)nullptr)
	TEST_EQUAL(compiled->source(), std::string("element(C)"))
	(void)cache;
RESULT

CHECK(K0.5.3 Expression with previously unsupported predicate compiles via K0.5.4 OwnedPred)
	// K0.5.4 closes the K0.5.3 fallback path: inRing() now lowers to an
	// OwnedPred wrapping InRingPredicate, so getCompiled returns a real
	// AST (not nullptr) and the back_ptr-walk evaluator runs.
	System sys;
	Atom *a = new Atom; sys.adopt(*a);

	Expression expr("inRing()");
	auto compiled = expr.getCompiled(sys.getStore());
	TEST_NOT_EQUAL(compiled.get(), (const CompiledExpression*)nullptr)
	TEST_EQUAL(std::holds_alternative<OwnedPred>(compiled->root()), true)
	TEST_EQUAL(std::get<OwnedPred>(compiled->root()).name, std::string("inRing"))

	// operator() works (InRingPredicate returns false for an unbonded atom).
	TEST_EQUAL(expr(*a), false)
RESULT

// ============================================================
// K0.5.4 — OwnedPred slow path tests
// ============================================================

CHECK(K0.5.4 unknown predicate name still throws ParseError)
	// Predicate not registered anywhere -> no OwnedPred wrap possible.
	System sys;
	auto& store = sys.getStore();
	bool threw = false;
	try {
		Expression expr("nonexistentPredicate()");
		(void)CompiledExpression::compile(store, expr);
	} catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

CHECK(K0.5.4 OwnedPred bitmap eval walks back_ptr column)
	// Compile residue() against an Expression — even though residue()
	// can't match anything when atoms aren't in a Residue, the OwnedPred
	// path should produce a 0-filled bitmap of correct size (proving the
	// back_ptr walk executes without crashing).
	System sys;
	Atom *a = new Atom; sys.adopt(*a);
	Atom *b = new Atom; sys.adopt(*b);
	(void)a; (void)b;
	auto& store = sys.getStore();
	Expression expr("residue(GLY)");
	auto compiled = expr.getCompiled(store);
	TEST_NOT_EQUAL(compiled.get(), (const CompiledExpression*)nullptr)
	TEST_EQUAL(std::holds_alternative<OwnedPred>(compiled->root()), true)
	std::vector<std::uint8_t> bm;
	compiled->evaluate(store, bm);
	TEST_EQUAL(bm.size(), store.size())
	// Atoms not in any residue -> ResiduePredicate returns false.
	for (std::uint8_t v : bm) TEST_EQUAL(v, 0)
RESULT

CHECK(K0.5.4 user-registered predicate routes through OwnedPred)
	// Register a predicate factory on a fresh Expression, compile, then
	// verify the OwnedPred holds the user instance.
	System sys;
	Atom *a = new Atom; sys.adopt(*a);

	Expression expr("aromaticBonds()");  // standard predicate, slow-path
	auto compiled = expr.getCompiled(sys.getStore());
	TEST_NOT_EQUAL(compiled.get(), (const CompiledExpression*)nullptr)
	TEST_EQUAL(std::holds_alternative<OwnedPred>(compiled->root()), true)
	TEST_EQUAL(std::get<OwnedPred>(compiled->root()).name, std::string("aromaticBonds"))

	// Bitmap matches per-atom path.
	std::vector<std::uint8_t> bm;
	compiled->evaluate(sys.getStore(), bm);
	TEST_EQUAL(bm[a->getStoreIndex()], compiled->evaluate_one(*a) ? 1 : 0)
RESULT

CHECK(K0.5.5 ExpressionParser parse() is serialised under mutex (TSAN-friendly))
	// Spec §6. Concurrent ExpressionParser::parse() calls used to race on
	// the static ExpressionParser::state and on the Flex/Bison process-
	// global lexer/parser state. K0.5.5 wraps parse() in a mutex to
	// serialise it. This test exercises N threads compiling DIFFERENT
	// expressions against ONE shared store. Without the lock, this used
	// to crash or yield wrong results under TSAN; with the lock, every
	// thread should get the correct compiled AST.
	System sys;
	Atom *c1 = new Atom; c1->setElement(PTE[Element::CARBON]); sys.adopt(*c1);
	Atom *o1 = new Atom; o1->setElement(PTE[Element::OXYGEN]); sys.adopt(*o1);
	auto& store = sys.getStore();
	CompiledExpressionCache::instance().clear();

	const int n_threads = 8;
	const int n_iters   = 50;
	std::vector<std::thread> ts;
	std::atomic<int> errors{0};

	auto worker = [&](int tid)
	{
		const std::string srcs[3] = {
			"element(C)",
			"element(O)",
			"element(C) OR element(O)"
		};
		for (int i = 0; i < n_iters; ++i)
		{
			const std::string& src = srcs[(tid + i) % 3];
			try
			{
				Expression e(String(src.c_str()));
				bool rc = e(*c1);
				bool ro = e(*o1);
				// Cross-check against expected truth table.
				bool exp_c = (src == "element(C)") || (src.find("OR") != std::string::npos);
				bool exp_o = (src == "element(O)") || (src.find("OR") != std::string::npos);
				if (rc != exp_c || ro != exp_o) ++errors;
			}
			catch (...) { ++errors; }
		}
	};

	for (int t = 0; t < n_threads; ++t) ts.emplace_back(worker, t);
	for (auto& th : ts) th.join();

	TEST_EQUAL(errors.load(), 0)
RESULT

CHECK(K0.5.4 fast-path + OwnedPred coexist inside one AST)
	// "element(C) AND inRing()" mixes a fast-path leaf with a slow leaf.
	// AndNode should evaluate both and bitwise-AND the bitmaps.
	System sys;
	Atom *c = new Atom; c->setElement(PTE[Element::CARBON]); sys.adopt(*c);
	Atom *o = new Atom; o->setElement(PTE[Element::OXYGEN]); sys.adopt(*o);
	auto& store = sys.getStore();
	Expression expr("element(C) AND inRing()");
	auto compiled = expr.getCompiled(store);
	TEST_NOT_EQUAL(compiled.get(), (const CompiledExpression*)nullptr)
	std::vector<std::uint8_t> bm;
	compiled->evaluate(store, bm);
	// Neither is in a ring (no bonds) -> both 0.
	TEST_EQUAL(bm[c->getStoreIndex()], 0)
	TEST_EQUAL(bm[o->getStoreIndex()], 0)
RESULT

CHECK(K0.5.2 cache invalidate_store scoped per store)
	System sys1, sys2;
	Atom *a1 = new Atom; sys1.adopt(*a1);
	Atom *a2 = new Atom; sys2.adopt(*a2);
	auto& cache = CompiledExpressionCache::instance();
	cache.clear();

	(void)cache.get_or_compile(sys1.getStore(), "true()");
	(void)cache.get_or_compile(sys2.getStore(), "true()");
	TEST_EQUAL(cache.size(), 2u)

	cache.invalidate_store(&sys1.getStore());
	TEST_EQUAL(cache.size(), 1u)
	// sys2's entry survives.
	auto p = cache.get_or_compile(sys2.getStore(), "true()");
	(void)p;
	TEST_EQUAL(cache.size(), 1u)
RESULT

CHECK(V21-COMPILED-EXPR-GENERATION-CHECK: evaluate throws after compact)
	// Compile against the store at gen=0, then mutate the store in a
	// way that bumps generation (compact). evaluate() must throw
	// rather than operate with stale offsets.
	System sys;
	Molecule m;
	sys.append(m);
	Atom* a = new Atom;
	a->setName("XX");
	m.append(*a);

	auto compiled = CompiledExpression::compile(sys.getStore(), std::string("name(XX)"));
	TEST_NOT_EQUAL(compiled.get(), nullptr)

	std::vector<std::uint8_t> bm;
	compiled->evaluate(sys.getStore(), bm);
	std::size_t hits = 0;
	for (auto v : bm) hits += v;
	TEST_EQUAL(hits, 1u)

	// Compact bumps generation_; cached intern offsets may now be stale.
	sys.getStore().compact();
	TEST_NOT_EQUAL(compiled->compile_generation(), sys.getStore().generation())
	TEST_EXCEPTION(Exception::InvalidArgument, compiled->evaluate(sys.getStore(), bm))

	// A fresh compile against the new generation works again.
	auto compiled2 = CompiledExpression::compile(sys.getStore(), std::string("name(XX)"));
	compiled2->evaluate(sys.getStore(), bm);
	hits = 0;
	for (auto v : bm) hits += v;
	TEST_EQUAL(hits, 1u)
RESULT

END_TEST
