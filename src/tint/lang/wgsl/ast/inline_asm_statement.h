#ifndef SRC_TINT_LANG_WGSL_AST_INLINE_ASM_STATEMENT_H_
#define SRC_TINT_LANG_WGSL_AST_INLINE_ASM_STATEMENT_H_

#include <string>
#include "src/tint/lang/wgsl/ast/statement.h"

namespace tint::wgsl::ast {

/// InlineAsmStatement is a class that represents an inline assembly statement in WGSL.
class InlineAsmStatement : public Statement {
 public:
  /// Constructor
  /// @param code the inline assembly code
  explicit InlineAsmStatement(const std::string& code);

  /// Destructor
  ~InlineAsmStatement() override;

  /// @returns the inline assembly code
  const std::string& GetCode() const;

 private:
  std::string code_;
};

}  // namespace tint::wgsl::ast

#endif  // SRC_TINT_LANG_WGSL_AST_INLINE_ASM_STATEMENT_H_
