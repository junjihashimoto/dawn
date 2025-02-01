#include "src/tint/lang/wgsl/ast/inline_asm_statement.h"

namespace tint::wgsl::ast {

InlineAsmStatement::InlineAsmStatement(const std::string& code) : code_(code) {}

InlineAsmStatement::~InlineAsmStatement() = default;

const std::string& InlineAsmStatement::GetCode() const {
    return code_;
}

}  // namespace tint::wgsl::ast
