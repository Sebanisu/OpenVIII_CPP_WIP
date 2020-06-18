##define array with path info.
add_library(paths
        paths.cpp
        paths.h)
set_target_properties(paths PROPERTIES LINKER_LANGUAGE CXX)
#
##test app for trying to extract a all files from the FIFLFS archives.
add_executable(TestExtractFIFLFS
        TestExtractFIFLFS.h TestExtractFIFLFS.cpp)
set_target_properties(TestExtractFIFLFS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractFIFLFS Archive paths)
#
##test app for trying to extract a single file from the FIFLFS archives.
add_executable(TestExtractSingleFIFLFS
        TestExtractSingleFIFLFS.h
        TestExtractSingleFIFLFS.cpp)
set_target_properties(TestExtractSingleFIFLFS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractSingleFIFLFS Archive paths)

#
##test app for trying to extract a all files from the ZZZ and FIFLFS archives.
add_executable(TestExtractZZZ
        TestExtractZZZ.h
        TestExtractZZZ.cpp)
set_target_properties(TestExtractZZZ PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractZZZ Archive paths)

#
##Test Archives loading struct
add_executable(TestArchives
        TestArchives.h TestArchives.cpp)
set_target_properties(TestArchives PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestArchives Archive paths)


#
##test searching threw zzz contents and nested fi fl fs files.
add_executable(TestExtractSingleZZZ
        TestExtractSingleZZZ.h TestExtractSingleZZZ.cpp)
set_target_properties(TestExtractSingleZZZ PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractSingleZZZ Archive paths)

##test kernel.bin
add_executable(TestKernel
        TestKernel.cpp TestKernel.h ../Kernel/Header.cpp ../Kernel/Header.h ../Kernel/AttackTypeT.cpp ../Kernel/AttackTypeT.h ../Kernel/EncodedStringOffset.cpp ../Kernel/EncodedStringOffset.h ../Kernel/AttackFlagsT.cpp ../Kernel/AttackFlagsT.h ../Kernel/ElementT.cpp ../Kernel/ElementT.h ../Kernel/TargetT.cpp ../Kernel/TargetT.h ../Kernel/SectionData.cpp ../Kernel/SectionData.h ../Kernel/SectionTypesT.cpp ../Kernel/SectionTypesT.h ../Kernel/BattleCommands.cpp ../Kernel/BattleCommands.h ../Kernel/StatGroup.cpp ../Kernel/StatGroup.h ../Kernel/GFGroup.cpp ../Kernel/GFGroup.h ../Kernel/BattleOnlyStatusesT.cpp ../Kernel/BattleOnlyStatusesT.h ../Kernel/PersistentStatusesT.cpp ../Kernel/PersistentStatusesT.h ../Kernel/JunctionStatusesT.cpp ../Kernel/JunctionStatusesT.h ../Kernel/Magic.cpp ../Kernel/Magic.h ../Kernel/JunctionableGFs.cpp ../Kernel/JunctionableGFs.h ../Kernel/EnemyAttacks.cpp ../Kernel/EnemyAttacks.h ../Kernel/Weapons.cpp ../Kernel/Weapons.h ../Kernel/RenzokukenFinishers.cpp ../Kernel/RenzokukenFinishers.h ../Kernel/Characters.cpp ../Kernel/Characters.h ../Kernel/BattleItems.cpp ../Kernel/BattleItems.h ../Kernel/NonBattleItems.cpp ../Kernel/NonBattleItems.h ../Kernel/CommandAbilitiesData.cpp ../Kernel/CommandAbilitiesData.h ../Kernel/JunctionAbilities.cpp ../Kernel/JunctionAbilities.h ../Kernel/CommandAbilities.cpp ../Kernel/CommandAbilities.h ../Kernel/StatPercentIncreaseAbilities.cpp ../Kernel/StatPercentIncreaseAbilities.h ../Kernel/CharacterAbilities.cpp ../Kernel/CharacterAbilities.h ../Kernel/PartyAbilities.cpp ../Kernel/PartyAbilities.h ../Kernel/GFAbilities.cpp ../Kernel/GFAbilities.h ../Kernel/MenuAbilities.cpp ../Kernel/MenuAbilities.h ../Kernel/TeamLagunaLimitBreaks.cpp ../Kernel/TeamLagunaLimitBreaks.h ../Kernel/QuistisBlueMagicLimitBreak.cpp ../Kernel/QuistisBlueMagicLimitBreak.h ../Kernel/IrvineShotLimitBreak.cpp ../Kernel/IrvineShotLimitBreak.h ../Kernel/ZellDuelLimitBreak.cpp ../Kernel/ZellDuelLimitBreak.h ../Kernel/RinoaLimitBreakPart1.cpp ../Kernel/RinoaLimitBreakPart1.h ../Kernel/RinoaLimitBreakPart2.cpp ../Kernel/RinoaLimitBreakPart2.h ../Kernel/SlotsArray.cpp ../Kernel/SlotsArray.h ../Kernel/Devour.cpp ../Kernel/Devour.h ../Kernel/Misc.cpp ../Kernel/Misc.h ../Kernel/NonJunctionableGFs.cpp ../Kernel/NonJunctionableGFs.h ../Kernel/AbilitiesT.cpp ../Kernel/AbilitiesT.h ../Kernel/UnlockerT.cpp ../Kernel/UnlockerT.h ../Kernel/UnlockableAbility.cpp ../Kernel/UnlockableAbility.h ../Kernel/CameraChangeT.cpp ../Kernel/CameraChangeT.h ../Kernel/RenzokukenFinishersT.cpp ../Kernel/RenzokukenFinishersT.h ../Kernel/CharactersT.cpp ../Kernel/CharactersT.h ../Kernel/GenderT.cpp ../Kernel/GenderT.h ../Kernel/StatGroupNoEVANoHIT.cpp ../Kernel/StatGroupNoEVANoHIT.h)
set_target_properties(TestKernel PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestKernel Archive paths)