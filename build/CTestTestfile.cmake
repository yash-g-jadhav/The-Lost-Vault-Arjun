# CMake generated Testfile for 
# Source directory: C:/MyGithub/The Lost Vault
# Build directory: C:/MyGithub/The Lost Vault/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(CollisionTests "C:/MyGithub/The Lost Vault/build/TheLostVaultTests.exe")
set_tests_properties(CollisionTests PROPERTIES  _BACKTRACE_TRIPLES "C:/MyGithub/The Lost Vault/CMakeLists.txt;115;add_test;C:/MyGithub/The Lost Vault/CMakeLists.txt;0;")
subdirs("_deps/glfw-build")
subdirs("_deps/glm-build")
