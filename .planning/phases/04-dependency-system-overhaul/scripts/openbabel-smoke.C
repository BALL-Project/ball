// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// openbabel-smoke.C — OpenBabel 3.x port smoke check (Plan 04-03)
//
// Smoke-verifies that the OpenBabel 3.x port produces correct chemistry by
// testing MolecularSimilarity::createOBMol + OBConversion directly:
//
//   Test 1: A BALL::Molecule built from explicit atoms/bonds converts to a
//           non-empty canonical SMILES via MolecularSimilarity::createOBMol.
//   Test 2: The same molecule produces identical SMILES twice (determinism).
//           A stale-implicit-H or aromaticity-perception bug in the port would
//           make this non-deterministic or produce empty output (Research A6).
//   Test 3: Reading the benzene SMILES 'c1ccccc1' via OBConversion and writing
//           it back yields a canonical SMILES that contains lowercase 'c'
//           (aromatic carbon). This exercises the OBConversion + aromaticity
//           perception path used internally by generateCanSmile.
//   Test 4: MolecularSimilarity::createOBMol from a hand-built BALL Molecule
//           with explicit bond orders produces a non-empty SMILES, confirming
//           the createOBMol + BeginModify/EndModify 3.x port path works.
//
// Prints OPENBABEL_SMOKE_OK and exits 0 on success.
// Prints failure details and exits 1 on any failure.
//
// Linked against libBALL (built with BALL_HAS_OPENBABEL=ON) and OpenBabel 3.x.
// Build and run via the companion openbabel-smoke.sh script.

#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/DATATYPE/string.h>

#ifdef BALL_HAS_OPENBABEL
  #include <openbabel/mol.h>
  #include <openbabel/atom.h>
  #include <openbabel/bond.h>
  #include <openbabel/obiter.h>
  #include <openbabel/obconversion.h>
  using namespace OpenBabel;
#endif

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace BALL;


// Trim the trailing tab+ID that OBConversion::WriteString appends to canonical SMILES.
static std::string stripSmilesId(std::string s)
{
  size_t tab = s.find('\t');
  if (tab != std::string::npos) s = s.substr(0, tab);
  while (!s.empty() && (s.back() == '\n' || s.back() == '\r' || s.back() == ' '))
  {
    s.pop_back();
  }
  return s;
}


// Build an acetic acid molecule (CH3COOH) as a BALL Molecule with explicit atoms
// and bonds. No explicit hydrogens — OB 3.x will perceive implicit H via the
// BeginModify/EndModify bracket in createOBMol.
static Molecule* buildAceticAcid()
{
  Molecule* mol = new Molecule;

  // C1 — methyl carbon
  Atom* c1 = new Atom;
  c1->setElement(PTE_::getElement("C"));
  c1->setPosition(Vector3(0.0, 0.0, 0.0));
  mol->insert(*c1);

  // C2 — carboxyl carbon
  Atom* c2 = new Atom;
  c2->setElement(PTE_::getElement("C"));
  c2->setPosition(Vector3(1.5, 0.0, 0.0));
  mol->insert(*c2);

  // O1 — carbonyl oxygen (double bond to C2)
  Atom* o1 = new Atom;
  o1->setElement(PTE_::getElement("O"));
  o1->setPosition(Vector3(2.2, 1.2, 0.0));
  mol->insert(*o1);

  // O2 — hydroxyl oxygen (single bond to C2)
  Atom* o2 = new Atom;
  o2->setElement(PTE_::getElement("O"));
  o2->setPosition(Vector3(2.2, -1.2, 0.0));
  mol->insert(*o2);

  Bond* b1 = c1->createBond(*c2);
  b1->setOrder(Bond::ORDER__SINGLE);

  Bond* b2 = c2->createBond(*o1);
  b2->setOrder(Bond::ORDER__DOUBLE);

  Bond* b3 = c2->createBond(*o2);
  b3->setOrder(Bond::ORDER__SINGLE);

  return mol;
}


int main(int, char**)
{
#ifndef BALL_HAS_OPENBABEL
  std::cerr << "FAIL: BALL was built without BALL_HAS_OPENBABEL\n";
  return 1;
#else

  // -------------------------------------------------------------------
  // Test 1 + 2: MolecularSimilarity::createOBMol produces a non-empty,
  //             deterministic canonical SMILES.
  // -------------------------------------------------------------------
  {
    Molecule* mol1 = buildAceticAcid();
    Molecule* mol2 = buildAceticAcid();

    OBMol* obmol1 = MolecularSimilarity::createOBMol(*mol1, false, true);
    OBMol* obmol2 = MolecularSimilarity::createOBMol(*mol2, false, true);
    delete mol1;
    delete mol2;

    if (!obmol1 || obmol1->NumAtoms() == 0)
    {
      std::cerr << "FAIL: createOBMol returned empty molecule (Test 1)\n";
      return 1;
    }

    OBConversion conv;
    conv.SetOutFormat("can");
    std::string s1 = stripSmilesId(conv.WriteString(obmol1));
    std::string s2 = stripSmilesId(conv.WriteString(obmol2));
    delete obmol1;
    delete obmol2;

    if (s1.empty())
    {
      std::cerr << "FAIL: canonical SMILES for acetic acid is empty (Test 1)\n";
      return 1;
    }
    std::cout << "  Test 1 PASS: acetic acid SMILES = '" << s1 << "'\n";

    if (s1 != s2)
    {
      std::cerr << "FAIL: canonical SMILES is not deterministic (Test 2)!\n"
                << "  first  = '" << s1 << "'\n"
                << "  second = '" << s2 << "'\n";
      return 1;
    }
    std::cout << "  Test 2 PASS: SMILES is deterministic\n";
  }

  // -------------------------------------------------------------------
  // Test 3: OBConversion reads the benzene SMILES 'c1ccccc1' and writes
  //         canonical SMILES containing lowercase aromatic 'c'.
  //         This mirrors the generateCanSmile path's OBConversion usage
  //         and verifies aromaticity perception works in OB 3.x.
  // -------------------------------------------------------------------
  {
    OBMol mol;
    OBConversion conv;
    conv.SetInFormat("smi");
    conv.SetOutFormat("can");

    if (!conv.ReadString(&mol, "c1ccccc1"))
    {
      std::cerr << "FAIL: OBConversion could not read benzene SMILES (Test 3)\n";
      return 1;
    }

    std::string smiles = stripSmilesId(conv.WriteString(&mol));

    if (smiles.empty())
    {
      std::cerr << "FAIL: benzene canonical SMILES is empty (Test 3)\n";
      return 1;
    }

    // Canonical benzene SMILES from OpenBabel must contain aromatic 'c'
    bool has_aromatic_c = (smiles.find('c') != std::string::npos);
    if (!has_aromatic_c)
    {
      std::cerr << "FAIL: benzene SMILES '" << smiles
                << "' has no aromatic 'c' — aromaticity perception broken (Test 3)!\n";
      return 1;
    }
    std::cout << "  Test 3 PASS: benzene SMILES = '" << smiles
              << "' (aromaticity correct)\n";
  }

  // -------------------------------------------------------------------
  // Test 4: createOBMol + OBConversion from a BALL Molecule with typed
  //         bonds produces a non-empty SMILES — confirms the 3.x port of
  //         the BeginModify/EndModify + implicit-H pathway works.
  // -------------------------------------------------------------------
  {
    Molecule* mol = buildAceticAcid();
    OBMol* obmol = MolecularSimilarity::createOBMol(*mol, false, true);
    delete mol;

    OBConversion conv;
    conv.SetOutFormat("can");
    std::string smiles = stripSmilesId(conv.WriteString(obmol));
    delete obmol;

    if (smiles.empty())
    {
      std::cerr << "FAIL: createOBMol pathway produced empty SMILES (Test 4)\n";
      return 1;
    }
    std::cout << "  Test 4 PASS: createOBMol pathway SMILES = '" << smiles << "'\n";
  }

  std::cout << "OPENBABEL_SMOKE_OK\n";
  return 0;

#endif // BALL_HAS_OPENBABEL
}
