/**
 *  Copyright (C) 2011
 *  University of Rochester Department of Computer Science
 *    and
 *  Lehigh University Department of Computer Science and Engineering
 *
 * License: Modified BSD
 *          Please see the file LICENSE.RSTM for licensing information
 */

#include <stm/config.h>
#if defined(STM_CPU_SPARC)
#include <sys/types.h>
#endif

/**
 *  Step 1:
 *    Include the configuration code for the harness, and the API code.
 */

#include <iostream>
#include <api/api.hpp>
#include "bmconfig.hpp"

/**
 *  We provide the option to build the entire benchmark in a single
 *  source. The bmconfig.hpp include defines all of the important functions
 *  that are implemented in this file, and bmharness.cpp defines the
 *  execution infrastructure.
 */
#ifdef SINGLE_SOURCE_BUILD
#include "bmharness.cpp"
#endif

/**
 *  Step 2:
 *    Declare the data type that will be stress tested via this benchmark.
 *    Also provide any functions that will be needed to manipulate the data
 *    type.  Take care to avoid unnecessary indirection.
 */

#include "DList.hpp"

/**
 *  Step 3:
 *    Declare an instance of the data type, and provide init, test, and verify
 *    functions
 */

/*** the list we will manipulate in the experiment */
DList* SET;

/*** Initialize the counter */
void bench_init()
{
  SET = (DList*)hcmalloc(sizeof(DList));
  SET->DListInit();
    // warm up the datastructure
    TM_BEGIN_FAST_INITIALIZATION();
    for (uint32_t i = 0; i < CFG.elements; i+=2){
      int u = i*1000;
      //      std::cout << "START " <<u << std::endl; 
        SET->insert(u TM_PARAM);
    }
    TM_END_FAST_INITIALIZATION();
}

/*** Run a bunch of increment transactions */
void bench_test(uintptr_t, uint32_t* seed)
{
    uint32_t act = rand_r(seed) % 100;

    if (act < CFG.lookpct) {
        TM_BEGIN(atomic) {
	  //val = rand_r(seed) % CFG.elements;
	  uint32_t val = rand_r(seed) % CFG.elements;
   
	  //val = rand_r(seed) % CFG.elements;
          //SET->remove(val TM_PARAM);
	  //val = rand_r(seed) % CFG.elements;
          //SET->insert(val TM_PARAM);
	  //std::cout << "read" << std::endl;
	  //for(int i=0; i<100; i++){
	  //val = i;//rand_r(seed) % CFG.elements;
	    //	      std::cout << "READ " << CFG.elements << " val " << val << std::endl;
	    //SET->lookup(val TM_PARAM);
	  //}
	  val = (rand_r(seed) % (CFG.elements*1000))+1000;
	  //std::cout << "---WRITE " << CFG.elements << " val " << val << std::endl;
	  //std::cout << val << std::endl;
          //SET->insert(val TM_PARAM);

        } TM_END;
	}/*
    else if (act < CFG.inspct) {
        TM_BEGIN(atomic) { 
	  //	  std::cout << "write" << std::endl;
            SET->insert(val TM_PARAM);
        } TM_END;
	}*/
    else {
        TM_BEGIN(atomic) {
	  //std::cout << "remove" << std::endl;
           uint32_t val = rand_r(seed) % CFG.elements;  
	   SET->remove(val TM_PARAM);
	   SET->insert(val TM_PARAM);
	   std::cout << val << std::endl;
        } TM_END;
	}
}

/*** Ensure the final state of the benchmark satisfies all invariants */
bool bench_verify() { return SET->isSane(); }

/**
 *  Step 4:
 *    Include the code that has the main() function, and the code for creating
 *    threads and calling the three above-named functions.  Don't forget to
 *    provide an arg reparser.
 */

/*** Deal with special names that map to different M values */
void bench_reparse()
{
    if (CFG.bmname == "") CFG.bmname = "DList";
}

