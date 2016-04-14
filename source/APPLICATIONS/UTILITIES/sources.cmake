# the directory name
SET(DIRECTORY source/APPLICATIONS/UTILITIES)

# list all filenames of the directory here
SET(EXECUTABLES_LIST
        add_hydrogens
        assign_bond_orders
        assign_charges_from_rules
        assign_radii_from_rules
        assign_typenames_from_rules
        assign_positions_from_template
        atomtyper
        calculate_RMSD
        clip_protein_around_ligand
        compute_docking_RMSD
        dcd2dcd
        export_fragment
        file_converter
        hin2mol2
        pdb2amber_naming
        pdb2dcd
        pdb2hin
        reconstruct_fragment
        solvent_accessibility
)

IF (BALL_HAS_FFTW)
        LIST(APPEND EXECUTABLES_LIST rigid_docking)

        IF (BALL_HAS_MPI)
                LIST(APPEND EXECUTABLES_LIST geometricFit_slave)
        ENDIF()
ENDIF()

SET(UTILITIES_EXECUTABLES ${TOOLS_EXECUTABLES} ${EXECUTABLES_LIST})

### add filenames to Visual Studio solution
SET(UTILITIES_SOURCES)
FOREACH(i ${UTILITIES_EXECUTABLES})
        LIST(APPEND UTILITIES_SOURCES "${i}")
ENDFOREACH(i)
SOURCE_GROUP("" FILES ${UTILITIES_SOURCES})
