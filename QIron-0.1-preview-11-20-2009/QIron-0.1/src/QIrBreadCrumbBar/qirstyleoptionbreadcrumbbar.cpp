#include "qirstyleoptionbreadcrumbbar.h"

QIR_BEGIN_NAMESPACE

////////////////////////////////
//QIrStyleOptionBreadCrumbIndicator
////////////////////////////////
QIrStyleOptionBreadCrumbIndicator::QIrStyleOptionBreadCrumbIndicator() : QStyleOption(Version,Type), 
isTruncated(false), hasLabel(true), usePseudoState(false), isValid(true), isFlat(false)
{
}

/////////////////////////////////
//QIrStyleOptionBreadCrumbLabel
/////////////////////////////////
QIrStyleOptionBreadCrumbLabel::QIrStyleOptionBreadCrumbLabel() : QStyleOption(Version,Type), text(QString()),
hasIndicator(true), usePseudoState(false), isValid(true), isFlat(false)
{
}

QIR_END_NAMESPACE
