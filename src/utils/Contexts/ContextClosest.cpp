/*
 * ContextClosest.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: nek3d
 */

#include "ContextClosest.h"

ContextClosest::ContextClosest()
: 	_haveTieMode(false),
	_ignoreOverlaps(false),
	_ignoreUpstream(false),
	_ignoreDownstream(false),
	_reportDistance(false),
	_signDistance(false),
	_haveStrandedDistMode(false),
	_diffNames(false),
	_tieMode(ALL_TIES),
	_strandedDistMode(REF_DIST),
	_multiDbMode(EACH_DB)
{
	// closest requires sorted input
	setSortedInput(true);

}

ContextClosest::~ContextClosest(){

}

bool ContextClosest::parseCmdArgs(int argc, char **argv, int skipFirstArgs){
	_argc = argc;
	_argv = argv;
	_skipFirstArgs = skipFirstArgs;
	if (_argc < 2) {
		setShowHelp(true);
		return false;
	}

	setProgram(_programNames[argv[0]]);

	_argsProcessed.resize(_argc - _skipFirstArgs, false);

	for (_i=_skipFirstArgs; _i < argc; _i++) {
		if (isUsed(_i - _skipFirstArgs)) {
			continue;
		}
		if (strcmp(_argv[_i], "-c") == 0) {
			//bypass intersect's use of the -c option, because -c
			//means writeCount for intersect, but means columns for map.
			if (!ContextBase::handle_c()) return false;
		}
        else if (strcmp(_argv[_i], "-d") == 0) {
           if (!handle_d()) return false;
        }
        else if (strcmp(_argv[_i], "-D") == 0) {
        	if (!handle_D()) return false;
        }
        else if (strcmp(_argv[_i], "-io") == 0) {
        	if (!handle_io()) return false;
        }
        else if (strcmp(_argv[_i], "-iu") == 0) {
        	if (!handle_iu()) return false;
        }
        else if (strcmp(_argv[_i], "-id") == 0) {
        	if (!handle_id()) return false;
        }
        else if (strcmp(_argv[_i], "-N") == 0) {
        	if (!handle_N()) return false;
        }
        else if (strcmp(_argv[_i], "-t") == 0) {
        	if (!handle_t()) return false;
        }
        else if (strcmp(_argv[_i], "-mdb") == 0) {
        	if (!handle_mdb()) return false;
        }

	}
	return ContextIntersect::parseCmdArgs(argc, argv, _skipFirstArgs);
}

bool ContextClosest::isValidState(){
	if (!ContextIntersect::isValidState()) return false;

   // make sure we have both input files
	if (_haveTieMode && (_tieMode != ALL_TIES) && (_tieMode != FIRST_TIE)
					&& (_tieMode != LAST_TIE)) {
		_errorMsg = "\n*****\n*****ERROR: Request \"all\" or \"first\" or \"last\" for Tie Mode (-t)\n*****\n";
		return false;
	}

	if (_haveStrandedDistMode && (_strandedDistMode != A_DIST) && (_strandedDistMode != B_DIST)
							 && (_strandedDistMode != REF_DIST)) {
		_errorMsg = "\n*****\n*****ERROR: Request \"a\" or \"b\" or \"ref\" for Stranded Distance Mode (-D)\n*****\n";
		return false;
	}

	if (_ignoreUpstream && _ignoreDownstream) {
		_errorMsg = "\n*****\n*****ERROR: Request either -iu OR -id, not both.\n*****\n";
		return false;
	}

	if ((_ignoreUpstream || _ignoreDownstream) && ! _haveStrandedDistMode) {
		_errorMsg  = "\n*****\n*****ERROR: When requesting -iu or -id, you also need to specify -D.\n*****\n";
		return false;
	}

	return true;
}

bool ContextClosest::handle_d() {
    _reportDistance = true;
    markUsed(_i - _skipFirstArgs);
    return true;
}

bool ContextClosest::handle_D() {
	bool strandError = false;
    if ((_i+1) < _argc) {
        _reportDistance = true;
        _signDistance = true;
        _haveStrandedDistMode = true;
        QuickString modeStr(_argv[_i + 1]);
        if (modeStr == "ref") {
        	_strandedDistMode = REF_DIST;
        } else if (modeStr == "a") {
        	_strandedDistMode = A_DIST;
        } else if (modeStr == "b") {
        	_strandedDistMode = B_DIST;
        } else {
        	strandError = true;
        }
    } else {
    	strandError = true;
    }
    if (!strandError) {
        markUsed(_i - _skipFirstArgs);
        _i++;
        markUsed(_i - _skipFirstArgs);
        return true;
    }
    _errorMsg = "*****ERROR: -D option must be followed with \"ref\", \"a\", or \"b\"";
    return false;
}

bool ContextClosest::handle_io() {
    _ignoreOverlaps = true;
    markUsed(_i - _skipFirstArgs);
    return true;
}

bool ContextClosest::handle_iu() {
	_ignoreUpstream = true;
    markUsed(_i - _skipFirstArgs);
    return true;
}

bool ContextClosest::handle_id() {
	_ignoreUpstream = true;
    markUsed(_i - _skipFirstArgs);
    return true;
}

bool ContextClosest::handle_N() {
    _diffNames = true;
    markUsed(_i - _skipFirstArgs);
    return true;
}

bool ContextClosest::handle_t()
{
	bool tieError = false;
    if ((_i+1) < _argc) {
        _haveTieMode = true;
        QuickString tieStr(_argv[_i+1]);
        if (tieStr == "all") {
        	_tieMode = ALL_TIES;
        } else if (tieStr == "first") {
        	_tieMode = FIRST_TIE;
        } else if (tieStr == "last") {
        	_tieMode = LAST_TIE;
        } else {
        	tieError = true;
        }
    } else {
    	tieError = true;
    }
    if (!tieError) {
        markUsed(_i - _skipFirstArgs);
        _i++;
        markUsed(_i - _skipFirstArgs);
        return true;
    }
	_errorMsg = "*****ERROR: Request \"all\", \"first\", \"last\" for Tie Mode (-t)";
	return false;
}

bool ContextClosest::handle_mdb()
{
	bool mdbError = false;
    if ((_i+1) < _argc) {
        QuickString mdbStr(_argv[_i+1]);
        if (mdbStr == "each") {
        	_multiDbMode = EACH_DB;
        } else if (mdbStr == "all") {
        	_multiDbMode = ALL_DBS;
        } else {
        	mdbError = true;
        }
    } else {
    	mdbError = true;
    }
    if (!mdbError) {
        markUsed(_i - _skipFirstArgs);
        _i++;
        markUsed(_i - _skipFirstArgs);
        return true;
    }
	_errorMsg = "*****ERROR: Request \"each\" or \"last\" for Multiple Database Mode (-mdb)";
	return false;
}
