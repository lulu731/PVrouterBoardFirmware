You are an autonomous coding agent working on this project.

# Your task:
1. @/docs/PRD.md : take a failing test
2. fix that single test by modifying stub or mock functions
4. Append progress to docs/progress.txt, update docs/PRD.md
5. commit the changes using /git-commit.md command without any user approval

# guidelines
1. You work in ACT mode
2. Each test is in a file test_somefeature.c. use 'ceedling test:somefeature'
3. You must not use cmock, only modify existing mock or stub files, see @/test/support.
4. You must not edit any project file in @/lib directory.
5. project.yml file is at project root directory.

Only work on ONE test per iteration.