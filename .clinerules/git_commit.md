---
  description: Rule for automatically committing changes made by Cline AI using conventional commits format.
  priority: high
  version: 1.1
---


# Git Conventional Commits

Rule for automatically committing changes made by CursorAI using conventional commits format.

## filters:
  - type: event
    pattern: "build_success"
  - type: file_change
    pattern: "*"

## actions:
  - type: execute
    command:
      ### Extract the change type and scope from the changes
      CHANGE_TYPE=""
      case "$CHANGE_DESCRIPTION" in
        *"feat"*|*"create"*|*"implement"*) CHANGE_TYPE="feat";;
        *"fix"*|*"correct"*|*"resolve"*) CHANGE_TYPE="fix";;
        *"system"*|*"dependencies"*) CHANGE_TYPE="build";;
        *"revert"*|*"previous"*|*"commit"*) CHANGE_TYPE="revert";;
        *"jenkins"*|*"CI"*|*"configuration"*|*"script"*) CHANGE_TYPE="ci";;
        *"refactor"*|*"restructure"*) CHANGE_TYPE="refactor";;
        *"test"*) CHANGE_TYPE="test";;
        *"doc"*|*"comment"*) CHANGE_TYPE="docs";;
        *"style"*|*"format"*) CHANGE_TYPE="style";;
        *"perf"*|*"optimize"*) CHANGE_TYPE="perf";;
        *"rule"*|*"tool"*|*"auxiliary"*) CHANGE_TYPE="chore";;
      esac

      ### Extract scope from file path
      SCOPE=$(dirname "$FILE" | tr '/' '-')

      ### Commit the changes
      git add "$FILE"
      git commit -m "$CHANGE_TYPE($SCOPE): $CHANGE_DESCRIPTION"

  - type: suggest
    message:
      Changes should be committed using conventional commits format:

      Format: <type>(<scope>): <description>

      Types:
      - feat - a new feature
      - fix - a bug fix
      - build - changes that affect the build system or external dependencies
      - chore - changes to the build process or auxiliary tools and libraries
      - ci - changes to our CI configuration files and scripts
      - docs - documentation only changes
      - perf - a code change that improves performance
      - refactor - a code change that neither fixes a bug nor adds a feature
      - revert - reverts a previous commit
      - style - changes that do not affect the meaning of the code
      - test - adding missing tests or correcting existing tests

      The scope should be derived from the file path or affected component.
      The description should be clear and concise, written in imperative mood.

## examples:
  - input:
      ### After adding a new function
      CHANGE_DESCRIPTION="add user authentication function"
      FILE="src/auth/login.c"
    output: "feat(src-auth): add user authentication function"

  - input:
      ### After fixing a bug
      CHANGE_DESCRIPTION="fix incorrect date parsing"
      FILE="lib/utils/date.c"
    output: "fix(lib-utils): fix incorrect date parsing"
