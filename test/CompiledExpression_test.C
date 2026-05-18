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

END_TEST
