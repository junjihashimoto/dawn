#include "src/tint/lang/msl/writer.h"
#include "src/tint/lang/wgsl/writer.h"
#include "gtest/gtest.h"

namespace tint::tests {

TEST(AsmTest, MSLAsm) {
    std::string code = "mov r0, r1";
    msl::asm(code);
    // Add assertions to verify the behavior of the MSL asm function
    // This is a placeholder implementation
    // Replace this with the actual test cases
    ASSERT_EQ(code, "mov r0, r1");
}

TEST(AsmTest, WGSLAsm) {
    std::string code = "mov r0, r1";
    wgsl::asm(code);
    // Add assertions to verify the behavior of the WGSL asm function
    // This is a placeholder implementation
    // Replace this with the actual test cases
    ASSERT_EQ(code, "mov r0, r1");
}

}  // namespace tint::tests
