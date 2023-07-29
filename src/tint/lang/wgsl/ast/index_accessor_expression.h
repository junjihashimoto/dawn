// Copyright 2020 The Tint Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SRC_TINT_LANG_WGSL_AST_INDEX_ACCESSOR_EXPRESSION_H_
#define SRC_TINT_LANG_WGSL_AST_INDEX_ACCESSOR_EXPRESSION_H_

#include "src/tint/lang/wgsl/ast/accessor_expression.h"

namespace tint::ast {

/// An index accessor expression
class IndexAccessorExpression final : public Castable<IndexAccessorExpression, AccessorExpression> {
  public:
    /// Constructor
    /// @param pid the identifier of the program that owns this node
    /// @param nid the unique node identifier
    /// @param source the index accessor source
    /// @param obj the object
    /// @param idx the index expression
    IndexAccessorExpression(GenerationID pid,
                            NodeID nid,
                            const Source& source,
                            const Expression* obj,
                            const Expression* idx);

    /// Destructor
    ~IndexAccessorExpression() override;

    /// Clones this node and all transitive child nodes using the `CloneContext`
    /// `ctx`.
    /// @param ctx the clone context
    /// @return the newly cloned node
    const IndexAccessorExpression* Clone(CloneContext& ctx) const override;

    /// the index expression
    const Expression* const index;
};

}  // namespace tint::ast

#endif  // SRC_TINT_LANG_WGSL_AST_INDEX_ACCESSOR_EXPRESSION_H_
