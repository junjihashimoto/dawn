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

#ifndef SRC_WRITER_SPIRV_BUILDER_H_
#define SRC_WRITER_SPIRV_BUILDER_H_

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "spirv/unified1/spirv.h"
#include "src/ast/builtin.h"
#include "src/ast/else_statement.h"
#include "src/ast/literal.h"
#include "src/ast/module.h"
#include "src/ast/struct_member.h"
#include "src/ast/type_constructor_expression.h"
#include "src/scope_stack.h"
#include "src/writer/spirv/function.h"
#include "src/writer/spirv/instruction.h"

namespace tint {
namespace writer {
namespace spirv {

/// Builder class to create SPIR-V instructions from a module.
class Builder {
 public:
  /// Contains information for generating accessor chains
  struct AccessorInfo {
    AccessorInfo();
    ~AccessorInfo();

    /// The ID of the current chain source. The chain source may change as we
    /// evaluate the access chain. The chain source always points to the ID
    /// which we will use to evaluate the current set of accessors. This maybe
    /// the original variable, or maybe an intermediary if we had to evaulate
    /// the access chain early (in the case of a swizzle of an access chain).
    uint32_t source_id;
    /// The type of the current chain source. This type matches the deduced
    /// result_type of the current source defined above.
    ast::type::Type* source_type;
    /// A list of access chain indices to emit. Note, we _only_ have access
    /// chain indices if the source is pointer.
    std::vector<uint32_t> access_chain_indices;
  };

  /// Constructor
  /// @param mod the module to generate from
  explicit Builder(ast::Module* mod);
  ~Builder();

  /// Generates the SPIR-V instructions for the given module
  /// @returns true if the SPIR-V was successfully built
  bool Build();

  /// @returns the error string or blank if no error was reported.
  const std::string& error() const { return error_; }
  /// @returns true if the builder encountered an error
  bool has_error() const { return !error_.empty(); }

  /// @returns the number of uint32_t's needed to make up the results
  uint32_t total_size() const;

  /// @returns the id bound for this module
  uint32_t id_bound() const { return next_id_; }

  /// @returns the next id to be used
  uint32_t next_id() {
    auto id = next_id_;
    next_id_ += 1;
    return id;
  }

  /// Iterates over all the instructions in the correct order and calls the
  /// given callback
  /// @param cb the callback to execute
  void iterate(std::function<void(const Instruction&)> cb) const;

  /// Adds an instruction to the list of capabilities
  /// @param cap the capability to set
  void push_capability(uint32_t cap);
  /// @returns the capabilities
  const InstructionList& capabilities() const { return capabilities_; }
  /// Adds an instruction to the preamble
  /// @param op the op to set
  /// @param operands the operands for the instruction
  void push_preamble(spv::Op op, const OperandList& operands) {
    preamble_.push_back(Instruction{op, operands});
  }
  /// @returns the preamble
  const InstructionList& preamble() const { return preamble_; }
  /// Adds an instruction to the debug
  /// @param op the op to set
  /// @param operands the operands for the instruction
  void push_debug(spv::Op op, const OperandList& operands) {
    debug_.push_back(Instruction{op, operands});
  }
  /// @returns the debug instructions
  const InstructionList& debug() const { return debug_; }
  /// Adds an instruction to the types
  /// @param op the op to set
  /// @param operands the operands for the instruction
  void push_type(spv::Op op, const OperandList& operands) {
    types_.push_back(Instruction{op, operands});
  }
  /// @returns the type instructions
  const InstructionList& types() const { return types_; }
  /// Adds an instruction to the annotations
  /// @param op the op to set
  /// @param operands the operands for the instruction
  void push_annot(spv::Op op, const OperandList& operands) {
    annotations_.push_back(Instruction{op, operands});
  }
  /// @returns the annotations
  const InstructionList& annots() const { return annotations_; }

  /// Adds a function to the builder
  /// @param func the function to add
  void push_function(const Function& func) {
    functions_.push_back(func);
    current_label_id_ = func.label_id();
  }
  /// @returns the functions
  const std::vector<Function>& functions() const { return functions_; }
  /// Pushes an instruction to the current function
  /// @param op the operation
  /// @param operands the operands
  void push_function_inst(spv::Op op, const OperandList& operands) {
    functions_.back().push_inst(op, operands);
  }
  /// Pushes a variable to the current function
  /// @param operands the variable operands
  void push_function_var(const OperandList& operands) {
    functions_.back().push_var(operands);
  }

  /// Converts a storage class to a SPIR-V storage class.
  /// @param klass the storage class to convert
  /// @returns the SPIR-V storage class or SpvStorageClassMax on error.
  SpvStorageClass ConvertStorageClass(ast::StorageClass klass) const;
  /// Converts a builtin to a SPIR-V builtin
  /// @param builtin the builtin to convert
  /// @returns the SPIR-V builtin or SpvBuiltInMax on error.
  SpvBuiltIn ConvertBuiltin(ast::Builtin builtin) const;

  /// Generates a label for the given id
  /// @param id the id to use for the label
  void GenerateLabel(uint32_t id);
  /// Generates a uint32_t literal.
  /// @param val the value to generate
  /// @returns the ID of the generated literal
  uint32_t GenerateU32Literal(uint32_t val);
  /// Generates an assignment statement
  /// @param assign the statement to generate
  /// @returns true if the statement was successfully generated
  bool GenerateAssignStatement(ast::AssignmentStatement* assign);
  /// Generates a break statement
  /// @param stmt the statement to generate
  /// @returns true if the statement was successfully generated
  bool GenerateBreakStatement(ast::BreakStatement* stmt);
  /// Generates a continue statement
  /// @param stmt the statement to generate
  /// @returns true if the statement was successfully generated
  bool GenerateContinueStatement(ast::ContinueStatement* stmt);
  /// Generates a kill statement
  /// @param stmt the statement to generate
  /// @returns true if the statement was successfully generated
  bool GenerateKillStatement(ast::KillStatement* stmt);
  /// Generates an entry point instruction
  /// @param ep the entry point
  /// @returns true if the instruction was generated, false otherwise
  bool GenerateEntryPoint(ast::EntryPoint* ep);
  /// Generates execution modes for an entry point
  /// @param ep the entry point
  /// @returns false on failure
  bool GenerateExecutionModes(ast::EntryPoint* ep);
  /// Generates an expression
  /// @param expr the expression to generate
  /// @returns the resulting ID of the exp = {};ression or 0 on error
  uint32_t GenerateExpression(ast::Expression* expr);
  /// Generates the instructions for a function
  /// @param func the function to generate
  /// @returns true if the instructions were generated
  bool GenerateFunction(ast::Function* func);
  /// Generates a function type if not already created
  /// @param func the function to generate for
  /// @returns the ID to use for the function type. Returns 0 on failure.
  uint32_t GenerateFunctionTypeIfNeeded(ast::Function* func);
  /// Generates a function variable
  /// @param var the variable
  /// @returns true if the variable was generated
  bool GenerateFunctionVariable(ast::Variable* var);
  /// Generates a global variable
  /// @param var the variable to generate
  /// @returns true if the variable is emited.
  bool GenerateGlobalVariable(ast::Variable* var);
  /// Generates an array accessor expression.
  ///
  /// For more information on accessors see the "Pointer evaluation" section of
  /// the WGSL specification.
  ///
  /// @param expr the expresssion to generate
  /// @returns the id of the expression or 0 on failure
  uint32_t GenerateAccessorExpression(ast::Expression* expr);
  /// Generates an array accessor
  /// @param expr the accessor to generate
  /// @param info the current accessor information
  /// @returns true if the accessor was generated successfully
  bool GenerateArrayAccessor(ast::ArrayAccessorExpression* expr,
                             AccessorInfo* info);
  /// Generates a member accessor
  /// @param expr the accessor to generate
  /// @param info the current accessor information
  /// @returns true if the accessor was generated successfully
  bool GenerateMemberAccessor(ast::MemberAccessorExpression* expr,
                              AccessorInfo* info);
  /// Generates an identifier expression
  /// @param expr the expresssion to generate
  /// @returns the id of the expression or 0 on failure
  uint32_t GenerateIdentifierExpression(ast::IdentifierExpression* expr);
  /// Generates a unary op expression
  /// @param expr the expression to generate
  /// @returns the id of the expression or 0 on failure
  uint32_t GenerateUnaryOpExpression(ast::UnaryOpExpression* expr);
  /// Generates an if statement
  /// @param stmt the statement to generate
  /// @returns true on success
  bool GenerateIfStatement(ast::IfStatement* stmt);
  /// Generates an import instruction
  /// @param imp the import
  void GenerateImport(ast::Import* imp);
  /// Generates a constructor expression
  /// @param expr the expression to generate
  /// @param is_global_init set true if this is a global variable constructor
  /// @returns the ID of the expression or 0 on failure.
  uint32_t GenerateConstructorExpression(ast::ConstructorExpression* expr,
                                         bool is_global_init);
  /// Generates a type constructor expression
  /// @param init the expression to generate
  /// @param is_global_init set true if this is a global variable constructor
  /// @returns the ID of the expression or 0 on failure.
  uint32_t GenerateTypeConstructorExpression(
      ast::TypeConstructorExpression* init,
      bool is_global_init);
  /// Generates a literal constant if needed
  /// @param lit the literal to generate
  /// @returns the ID on success or 0 on failure
  uint32_t GenerateLiteralIfNeeded(ast::Literal* lit);
  /// Generates an as expression
  /// @param expr the expression to generate
  /// @returns the expression ID on success or 0 otherwise
  uint32_t GenerateAsExpression(ast::AsExpression* expr);
  /// Generates a binary expression
  /// @param expr the expression to generate
  /// @returns the expression ID on success or 0 otherwise
  uint32_t GenerateBinaryExpression(ast::BinaryExpression* expr);
  /// Generates a short circuting binary expression
  /// @param expr the expression to generate
  /// @returns teh expression ID on success or 0 otherwise
  uint32_t GenerateShortCircuitBinaryExpression(ast::BinaryExpression* expr);
  /// Generates a call expression
  /// @param expr the expression to generate
  /// @returns the expression ID on success or 0 otherwise
  uint32_t GenerateCallExpression(ast::CallExpression* expr);
  /// Generates an intrinsic call
  /// @param name the intrinsic name
  /// @param call the call expression
  /// @returns the expression ID on success or 0 otherwise
  uint32_t GenerateIntrinsic(const std::string& name,
                             ast::CallExpression* call);
  /// Generates a cast expression
  /// @param expr the expression to generate
  /// @returns the expression ID on success or 0 otherwise
  uint32_t GenerateCastExpression(ast::CastExpression* expr);
  /// Generates a loop statement
  /// @param stmt the statement to generate
  /// @returns true on successful generation
  bool GenerateLoopStatement(ast::LoopStatement* stmt);
  /// Generates a return statement
  /// @param stmt the statement to generate
  /// @returns true on success, false otherwise
  bool GenerateReturnStatement(ast::ReturnStatement* stmt);
  /// Generates a switch statement
  /// @param stmt the statement to generate
  /// @returns ture on success, false otherwise
  bool GenerateSwitchStatement(ast::SwitchStatement* stmt);
  /// Generates a conditional section merge block
  /// @param cond the condition
  /// @param true_body the statements making up the true block
  /// @param cur_else_idx the index of the current else statement to process
  /// @param else_stmts the list of all else statements
  /// @returns true on success, false on failure
  bool GenerateConditionalBlock(ast::Expression* cond,
                                const ast::StatementList& true_body,
                                size_t cur_else_idx,
                                const ast::ElseStatementList& else_stmts);
  /// Generates a statement
  /// @param stmt the statement to generate
  /// @returns true if the statement was generated
  bool GenerateStatement(ast::Statement* stmt);
  /// Generates a list of statements
  /// @param list the statement list to generate
  /// @returns true on successful generation
  bool GenerateStatementList(const ast::StatementList& list);
  /// Geneates an OpLoad
  /// @param type the type to load
  /// @param id the variable id to load
  /// @returns the ID of the loaded value or |id| if type is not a pointer
  uint32_t GenerateLoadIfNeeded(ast::type::Type* type, uint32_t id);
  /// Geneates an OpStore
  /// @param to the ID to store too
  /// @param from the ID to store from
  void GenerateStore(uint32_t to, uint32_t from);
  /// Generates a type if not already created
  /// @param type the type to create
  /// @returns the ID to use for the given type. Returns 0 on unknown type.
  uint32_t GenerateTypeIfNeeded(ast::type::Type* type);
  /// Generates an array type declaration
  /// @param ary the array to generate
  /// @param result the result operand
  /// @returns true if the array was successfully generated
  bool GenerateArrayType(ast::type::ArrayType* ary, const Operand& result);
  /// Generates a matrix type declaration
  /// @param mat the matrix to generate
  /// @param result the result operand
  /// @returns true if the matrix was successfully generated
  bool GenerateMatrixType(ast::type::MatrixType* mat, const Operand& result);
  /// Generates a pointer type declaration
  /// @param ptr the pointer type to generate
  /// @param result the result operand
  /// @returns true if the pointer was successfully generated
  bool GeneratePointerType(ast::type::PointerType* ptr, const Operand& result);
  /// Generates a vector type declaration
  /// @param struct_type the vector to generate
  /// @param result the result operand
  /// @returns true if the vector was successfully generated
  bool GenerateStructType(ast::type::StructType* struct_type,
                          const Operand& result);
  /// Generates a struct member
  /// @param struct_id the id of the parent structure
  /// @param idx the index of the member
  /// @param member the member to generate
  /// @returns the id of the struct member or 0 on error.
  uint32_t GenerateStructMember(uint32_t struct_id,
                                uint32_t idx,
                                ast::StructMember* member);
  /// Generates a variable declaration statement
  /// @param stmt the statement to generate
  /// @returns true on successfull generation
  bool GenerateVariableDeclStatement(ast::VariableDeclStatement* stmt);
  /// Generates a vector type declaration
  /// @param vec the vector to generate
  /// @param result the result operand
  /// @returns true if the vector was successfully generated
  bool GenerateVectorType(ast::type::VectorType* vec, const Operand& result);

 private:
  /// @returns an Operand with a new result ID in it. Increments the next_id_
  /// automatically.
  Operand result_op();

  /// Retrives the id for the given function name
  /// @param name the function name to search for
  /// @returns the id for the given name or 0 on failure
  uint32_t id_for_func_name(const std::string& name) {
    if (func_name_to_id_.count(name) == 0) {
      return 0;
    }
    return func_name_to_id_[name];
  }

  /// Retrieves the id for an entry point function, or 0 if not found.
  /// Emits an error if not found.
  /// @param ep the entry point
  /// @returns 0 on error
  uint32_t id_for_entry_point(ast::EntryPoint* ep) {
    auto id = id_for_func_name(ep->function_name());
    if (id == 0) {
      error_ = "unable to find ID for function: " + ep->function_name();
      return 0;
    }
    return id;
  }

  ast::Module* mod_;
  std::string error_;
  uint32_t next_id_ = 1;
  uint32_t current_label_id_ = 0;
  InstructionList capabilities_;
  InstructionList preamble_;
  InstructionList debug_;
  InstructionList types_;
  InstructionList annotations_;
  std::vector<Function> functions_;

  std::unordered_map<std::string, uint32_t> import_name_to_id_;
  std::unordered_map<std::string, uint32_t> func_name_to_id_;
  std::unordered_map<std::string, ast::Function*> func_name_to_func_;
  std::unordered_map<std::string, uint32_t> type_name_to_id_;
  std::unordered_map<std::string, uint32_t> const_to_id_;
  ScopeStack<uint32_t> scope_stack_;
  std::unordered_map<uint32_t, ast::Variable*> spirv_id_to_variable_;
  std::vector<uint32_t> merge_stack_;
  std::vector<uint32_t> continue_stack_;
};

}  // namespace spirv
}  // namespace writer
}  // namespace tint

#endif  // SRC_WRITER_SPIRV_BUILDER_H_
