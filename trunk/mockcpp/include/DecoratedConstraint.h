
#ifndef __MOCKCPP_DECORATED_CONSTRAINT_H
#define __MOCKCPP_DECORATED_CONSTRAINT_H

#include <mockcpp.h>
#include <Constraint.h>

MOCKCPP_NS_START


class DecoratedConstraint : public Constraint
{
public:

    DecoratedConstraint(Constraint* constraint);

    ~DecoratedConstraint();

    bool eval(const RefAny& val) const;
    std::string toString() const;

    virtual bool evalSelf(const RefAny& val) const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getTypeAndValueString() const = 0;

protected:

    bool hasDecoratedConstraint() const;

private:

    std::string getDecoratedConstraintString() const;

private:

    Constraint* decoratedConstraint;
};

MOCKCPP_NS_END

#endif

