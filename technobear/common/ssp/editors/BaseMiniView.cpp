#include "BaseMiniView.h"

#include "ssp/BaseProcessor.h"

namespace ssp {

BaseMiniView::BaseMiniView(BaseProcessor *p) : base_type(p, true) {
}

}  // namespace ssp