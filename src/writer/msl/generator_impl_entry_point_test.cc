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

#include "gtest/gtest.h"
#include "src/ast/assignment_statement.h"
#include "src/ast/decorated_variable.h"
#include "src/ast/entry_point.h"
#include "src/ast/identifier_expression.h"
#include "src/ast/location_decoration.h"
#include "src/ast/module.h"
#include "src/ast/type/f32_type.h"
#include "src/ast/type/i32_type.h"
#include "src/ast/variable.h"
#include "src/context.h"
#include "src/type_determiner.h"
#include "src/writer/msl/generator_impl.h"

namespace tint {
namespace writer {
namespace msl {
namespace {

using MslGeneratorImplTest = testing::Test;

TEST_F(MslGeneratorImplTest, EmitEntryPoint_Vertex_Input) {
  // [[location 0]] var<in> foo : f32;
  // [[location 1]] var<in> bar : i32;
  //
  // struct vtx_main_in {
  //   float foo [[attribute(0)]];
  //   int bar [[attribute(1)]];
  // };

  ast::type::F32Type f32;
  ast::type::I32Type i32;

  auto foo_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("foo", ast::StorageClass::kInput, &f32));

  ast::VariableDecorationList decos;
  decos.push_back(std::make_unique<ast::LocationDecoration>(0));
  foo_var->set_decorations(std::move(decos));

  auto bar_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("bar", ast::StorageClass::kInput, &i32));
  decos.push_back(std::make_unique<ast::LocationDecoration>(1));
  bar_var->set_decorations(std::move(decos));

  Context ctx;
  ast::Module mod;
  TypeDeterminer td(&ctx, &mod);
  td.RegisterVariableForTesting(foo_var.get());
  td.RegisterVariableForTesting(bar_var.get());

  mod.AddGlobalVariable(std::move(foo_var));
  mod.AddGlobalVariable(std::move(bar_var));

  ast::VariableList params;
  auto func =
      std::make_unique<ast::Function>("vtx_main", std::move(params), &f32);

  ast::StatementList body;
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("foo"),
      std::make_unique<ast::IdentifierExpression>("foo")));
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("bar"),
      std::make_unique<ast::IdentifierExpression>("bar")));
  func->set_body(std::move(body));

  mod.AddFunction(std::move(func));

  auto ep = std::make_unique<ast::EntryPoint>(ast::PipelineStage::kVertex,
                                              "main", "vtx_main");
  auto* ep_ptr = ep.get();

  mod.AddEntryPoint(std::move(ep));

  ASSERT_TRUE(td.Determine()) << td.error();

  GeneratorImpl g;
  g.set_module_for_testing(&mod);
  ASSERT_TRUE(g.EmitEntryPoint(ep_ptr)) << g.error();
  EXPECT_EQ(g.result(), R"(struct vtx_main_in {
  float foo [[attribute(0)]];
  int bar [[attribute(1)]];
};

)");
}

TEST_F(MslGeneratorImplTest, EmitEntryPoint_Vertex_Output) {
  // [[location 0]] var<out> foo : f32;
  // [[location 1]] var<out> bar : i32;
  //
  // struct vtx_main_out {
  //   float foo [[user(locn0)]];
  //   int bar [[user(locn1)]];
  // };

  ast::type::F32Type f32;
  ast::type::I32Type i32;

  auto foo_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("foo", ast::StorageClass::kOutput, &f32));

  ast::VariableDecorationList decos;
  decos.push_back(std::make_unique<ast::LocationDecoration>(0));
  foo_var->set_decorations(std::move(decos));

  auto bar_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("bar", ast::StorageClass::kOutput, &i32));
  decos.push_back(std::make_unique<ast::LocationDecoration>(1));
  bar_var->set_decorations(std::move(decos));

  Context ctx;
  ast::Module mod;
  TypeDeterminer td(&ctx, &mod);
  td.RegisterVariableForTesting(foo_var.get());
  td.RegisterVariableForTesting(bar_var.get());

  mod.AddGlobalVariable(std::move(foo_var));
  mod.AddGlobalVariable(std::move(bar_var));

  ast::VariableList params;
  auto func =
      std::make_unique<ast::Function>("vtx_main", std::move(params), &f32);

  ast::StatementList body;
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("foo"),
      std::make_unique<ast::IdentifierExpression>("foo")));
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("bar"),
      std::make_unique<ast::IdentifierExpression>("bar")));
  func->set_body(std::move(body));

  mod.AddFunction(std::move(func));

  auto ep = std::make_unique<ast::EntryPoint>(ast::PipelineStage::kVertex,
                                              "main", "vtx_main");
  auto* ep_ptr = ep.get();

  mod.AddEntryPoint(std::move(ep));

  ASSERT_TRUE(td.Determine()) << td.error();

  GeneratorImpl g;
  g.set_module_for_testing(&mod);
  ASSERT_TRUE(g.EmitEntryPoint(ep_ptr)) << g.error();
  EXPECT_EQ(g.result(), R"(struct vtx_main_out {
  float foo [[user(locn0)]];
  int bar [[user(locn1)]];
};

)");
}

TEST_F(MslGeneratorImplTest, EmitEntryPoint_Fragment_Input) {
  // [[location 0]] var<in> foo : f32;
  // [[location 1]] var<in> bar : i32;
  //
  // struct frag_main_in {
  //   float foo [[user(locn0)]];
  //   int bar [[user(locn1)]];
  // };

  ast::type::F32Type f32;
  ast::type::I32Type i32;

  auto foo_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("foo", ast::StorageClass::kInput, &f32));

  ast::VariableDecorationList decos;
  decos.push_back(std::make_unique<ast::LocationDecoration>(0));
  foo_var->set_decorations(std::move(decos));

  auto bar_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("bar", ast::StorageClass::kInput, &i32));
  decos.push_back(std::make_unique<ast::LocationDecoration>(1));
  bar_var->set_decorations(std::move(decos));

  Context ctx;
  ast::Module mod;
  TypeDeterminer td(&ctx, &mod);
  td.RegisterVariableForTesting(foo_var.get());
  td.RegisterVariableForTesting(bar_var.get());

  mod.AddGlobalVariable(std::move(foo_var));
  mod.AddGlobalVariable(std::move(bar_var));

  ast::VariableList params;
  auto func =
      std::make_unique<ast::Function>("frag_main", std::move(params), &f32);

  ast::StatementList body;
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("foo"),
      std::make_unique<ast::IdentifierExpression>("foo")));
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("bar"),
      std::make_unique<ast::IdentifierExpression>("bar")));
  func->set_body(std::move(body));

  mod.AddFunction(std::move(func));

  auto ep = std::make_unique<ast::EntryPoint>(ast::PipelineStage::kFragment,
                                              "main", "frag_main");
  auto* ep_ptr = ep.get();

  mod.AddEntryPoint(std::move(ep));

  ASSERT_TRUE(td.Determine()) << td.error();

  GeneratorImpl g;
  g.set_module_for_testing(&mod);
  ASSERT_TRUE(g.EmitEntryPoint(ep_ptr)) << g.error();
  EXPECT_EQ(g.result(), R"(struct frag_main_in {
  float foo [[user(locn0)]];
  int bar [[user(locn1)]];
};

)");
}

TEST_F(MslGeneratorImplTest, EmitEntryPoint_Fragment_Output) {
  // [[location 0]] var<out> foo : f32;
  // [[location 1]] var<out> bar : i32;
  //
  // struct frag_main_out {
  //   float foo [[color(0)]];
  //   int bar [[color(1)]];
  // };

  ast::type::F32Type f32;
  ast::type::I32Type i32;

  auto foo_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("foo", ast::StorageClass::kOutput, &f32));

  ast::VariableDecorationList decos;
  decos.push_back(std::make_unique<ast::LocationDecoration>(0));
  foo_var->set_decorations(std::move(decos));

  auto bar_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("bar", ast::StorageClass::kOutput, &i32));
  decos.push_back(std::make_unique<ast::LocationDecoration>(1));
  bar_var->set_decorations(std::move(decos));

  Context ctx;
  ast::Module mod;
  TypeDeterminer td(&ctx, &mod);
  td.RegisterVariableForTesting(foo_var.get());
  td.RegisterVariableForTesting(bar_var.get());

  mod.AddGlobalVariable(std::move(foo_var));
  mod.AddGlobalVariable(std::move(bar_var));

  ast::VariableList params;
  auto func =
      std::make_unique<ast::Function>("frag_main", std::move(params), &f32);

  ast::StatementList body;
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("foo"),
      std::make_unique<ast::IdentifierExpression>("foo")));
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("bar"),
      std::make_unique<ast::IdentifierExpression>("bar")));
  func->set_body(std::move(body));

  mod.AddFunction(std::move(func));

  auto ep = std::make_unique<ast::EntryPoint>(ast::PipelineStage::kFragment,
                                              "main", "frag_main");
  auto* ep_ptr = ep.get();

  mod.AddEntryPoint(std::move(ep));

  ASSERT_TRUE(td.Determine()) << td.error();

  GeneratorImpl g;
  g.set_module_for_testing(&mod);
  ASSERT_TRUE(g.EmitEntryPoint(ep_ptr)) << g.error();
  EXPECT_EQ(g.result(), R"(struct frag_main_out {
  float foo [[color(0)]];
  int bar [[color(1)]];
};

)");
}

TEST_F(MslGeneratorImplTest, EmitEntryPoint_Compute_Input) {
  // [[location 0]] var<in> foo : f32;
  // [[location 1]] var<in> bar : i32;
  //
  // -> Error, not allowed

  ast::type::F32Type f32;
  ast::type::I32Type i32;

  auto foo_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("foo", ast::StorageClass::kInput, &f32));

  ast::VariableDecorationList decos;
  decos.push_back(std::make_unique<ast::LocationDecoration>(0));
  foo_var->set_decorations(std::move(decos));

  auto bar_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("bar", ast::StorageClass::kInput, &i32));
  decos.push_back(std::make_unique<ast::LocationDecoration>(1));
  bar_var->set_decorations(std::move(decos));

  Context ctx;
  ast::Module mod;
  TypeDeterminer td(&ctx, &mod);
  td.RegisterVariableForTesting(foo_var.get());
  td.RegisterVariableForTesting(bar_var.get());

  mod.AddGlobalVariable(std::move(foo_var));
  mod.AddGlobalVariable(std::move(bar_var));

  ast::VariableList params;
  auto func =
      std::make_unique<ast::Function>("comp_main", std::move(params), &f32);

  ast::StatementList body;
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("foo"),
      std::make_unique<ast::IdentifierExpression>("foo")));
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("bar"),
      std::make_unique<ast::IdentifierExpression>("bar")));
  func->set_body(std::move(body));

  mod.AddFunction(std::move(func));

  auto ep = std::make_unique<ast::EntryPoint>(ast::PipelineStage::kCompute,
                                              "main", "comp_main");
  auto* ep_ptr = ep.get();

  mod.AddEntryPoint(std::move(ep));

  ASSERT_TRUE(td.Determine()) << td.error();

  GeneratorImpl g;
  g.set_module_for_testing(&mod);
  ASSERT_FALSE(g.EmitEntryPoint(ep_ptr)) << g.error();
  EXPECT_EQ(g.error(), R"(invalid location variable for pipeline stage)");
}

TEST_F(MslGeneratorImplTest, EmitEntryPoint_Compute_Output) {
  // [[location 0]] var<out> foo : f32;
  // [[location 1]] var<out> bar : i32;
  //
  // -> Error not allowed

  ast::type::F32Type f32;
  ast::type::I32Type i32;

  auto foo_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("foo", ast::StorageClass::kOutput, &f32));

  ast::VariableDecorationList decos;
  decos.push_back(std::make_unique<ast::LocationDecoration>(0));
  foo_var->set_decorations(std::move(decos));

  auto bar_var = std::make_unique<ast::DecoratedVariable>(
      std::make_unique<ast::Variable>("bar", ast::StorageClass::kOutput, &i32));
  decos.push_back(std::make_unique<ast::LocationDecoration>(1));
  bar_var->set_decorations(std::move(decos));

  Context ctx;
  ast::Module mod;
  TypeDeterminer td(&ctx, &mod);
  td.RegisterVariableForTesting(foo_var.get());
  td.RegisterVariableForTesting(bar_var.get());

  mod.AddGlobalVariable(std::move(foo_var));
  mod.AddGlobalVariable(std::move(bar_var));

  ast::VariableList params;
  auto func =
      std::make_unique<ast::Function>("comp_main", std::move(params), &f32);

  ast::StatementList body;
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("foo"),
      std::make_unique<ast::IdentifierExpression>("foo")));
  body.push_back(std::make_unique<ast::AssignmentStatement>(
      std::make_unique<ast::IdentifierExpression>("bar"),
      std::make_unique<ast::IdentifierExpression>("bar")));
  func->set_body(std::move(body));

  mod.AddFunction(std::move(func));

  auto ep = std::make_unique<ast::EntryPoint>(ast::PipelineStage::kCompute,
                                              "main", "comp_main");
  auto* ep_ptr = ep.get();

  mod.AddEntryPoint(std::move(ep));

  ASSERT_TRUE(td.Determine()) << td.error();

  GeneratorImpl g;
  g.set_module_for_testing(&mod);
  ASSERT_FALSE(g.EmitEntryPoint(ep_ptr)) << g.error();
  EXPECT_EQ(g.error(), R"(invalid location variable for pipeline stage)");
}

}  // namespace
}  // namespace msl
}  // namespace writer
}  // namespace tint
