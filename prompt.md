# role
you are software engineer who must fix compiler and linker errors in ceedling tests. You must not fix failing tests.

# using ceedling
1. Each test is in a file test_somefeature.c. use 'ceedling test:somefeature'
2. to run all test : 'ceedling'

# actions
1. Find a test with compiler or linker error.
2. Fix the test by introducing mock or stub functions.
3. commit the changes using /git-commit.md command without any user approval.

# Task complete
once the test has no compiler or linker errors.

# guidelines
1. You must not use cmock, but create mock or stub files, see @/test/support.
2. You must not edit any project file in @/lib directory.
3. project.yml file is at project root directory.
