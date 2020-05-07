#execute_process(COMMAND ./net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/prs-calc --verbose --output X830-scores.test.txt --weight X830-weights.clumped --dosage dosages_list.txt OUTPUT_VARIABLE out ERROR_VARIABLE err RESULT_VARIABLE prs_exit_code)

#execute_process(COMMAND ${PRSCALC} --verbose --output X830-scores.test.txt --weight /net/hunt/home/kotah/prs-server-beta/weights/merged-weights/merge-test.txt  --dosage dosages_list.txt RESULT_VARIABLE prs_exit_code)

execute_process(COMMAND ${PRSCALC} RESULT_VARIABLE prs_exit_code)

if(prs_exit_code)
   message("err='${err}'")
   message("out='${out}'")
   message("prs_exit_code='${prs_exit_code}'")
   message(FATAL_ERROR "PRSCALC failed.")
endif()

execute_process(COMMAND ${TESTPRS} good/X830.REFERENCE.txt X830-scores.test.txt 0.05 RESULT_VARIABLE prs_exit_code)
if(prs_exit_code)
   message(FATAL_ERROR "PRS didn't replicate results.")
endif()
