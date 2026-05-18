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

END_TEST
