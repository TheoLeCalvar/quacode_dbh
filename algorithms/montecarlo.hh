/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *    Vincent Barichard <Vincent.Barichard@univ-angers.fr>
 *
 *  Copyright:
 *    Vincent Barichard, 2015
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __MONTECARLO_HH__
#define __MONTECARLO_HH__

#include <vector>
#include <string>
#include <quacode/asyncalgo.hh>

class MonteCarlo : public AsyncAlgo {
    /// Copy constructor set private to disable it.
    MonteCarlo(const MonteCarlo&);

    /// Stores the number of variables
    int mNbVars;
    /// Vector of name of variables
    std::vector< std::string > mVarNames;
    /// Vector of domains of variables
    std::vector< Interval > mDomains;

    /// Constraints of the problem
    struct MX {
        int coeff;
        int iVar;
    };
    typedef std::vector< MX > TConstraint;;
    std::vector< TConstraint > mLinearConstraints;
    std::vector< TConstraint > mTimesConstraints;

    /// Function that returns the index of the variable
    /// \a name in our data structure. It returns -1 if the variable
    /// is undefined
    int getIdxVar(const std::string& name) const;

    /// Eval the sum of all constraints of the problem
    /// with the given instance \a instance
    unsigned long int evalConstraints(const std::vector<int>& instance) const;

    /// Fill the given instance \a instance with
    /// random numbers
    void generateInstance(std::vector<int>& instance);

public:
    /// Main constructor, \a killThread is set to false
    /// if we want that the main thread (Quacode) waits for
    /// the end of the asynchronous working thread
    MonteCarlo(bool killThread = true);

    /// Function called when a new variable \a var named \a name
    /// is created at position \a idx in the binder.
    /// \a t is the type of the variable, and
    /// \a min and \a max are the lower and upper bounds of the domain
    virtual void newVarCreated(int idx, Gecode::TQuantifier q, const std::string& name, TVarType t, int min, int max);
    /// Function called when a new auxiliary  variable \a var named \a name
    /// is created. \a t is the type of the variable, and
    /// \a min and \a max are the lower and upper bounds of the domain
    virtual void newAuxVarCreated(const std::string& name, TVarType t, int min, int max);

    /// Function called when a new 'n*v0*v1 <cmp> v2' constraint is posted
    virtual void postedTimes(int n, const std::string& v0, const std::string& v1, TComparisonType cmp, const std::string& v2);
    /// Function called when a new 'SUM_i n_i*v_i <cmp> v0' constraint is posted
    virtual void postedLinear(const std::vector<Monom>& poly, TComparisonType cmp, const std::string& v0);

    /// Function called when a new choice (\a iVar = variable index in the binder,
    /// \a min and \a max are the lower and upper bounds of the value) during search
    virtual void newChoice(int iVar, int min, int max);
    /// Function called when a new promising scenario is discovered during search
    virtual void newPromisingScenario(const TScenario& instance);
    /// Function called when the search ends with a successfull strategy
    virtual void strategyFound();
    /// Function called when a failure occured during search
    virtual void newFailure();
    /// Function called when the search ends with a global failure, problem unfeasible
    virtual void globalFailure();

    /// Function executed when the thread starts
    virtual void parallelTask(void);

    // Main destructor
    virtual ~MonteCarlo();
};

#endif