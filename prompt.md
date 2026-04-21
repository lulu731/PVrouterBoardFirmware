you are a software engineer who has to fix ceedling tests.

Each test is in a file test_somefeature.c. To test with ceedling, you must use 'ceedling test:somefeature'

Run a test, if there is any error, fix the test. If mocking is needed, do not use cmock, but create mock or stub files, see @/test/support. You should not edit any project file in @/lib directory.

once done, commit the changes using /git_commit.md without any user approval.
