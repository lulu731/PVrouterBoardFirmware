---
description: Workflow for committing changes using conventional commits format with automatic type detection.
priority: high
version: 2.0
---

# Git Conventional Commits

Workflow for automatically committing changes made by Cline AI using conventional commits format.

## Commit Format

Format: `<type>(<scope>): <description>`

Types:
- `feat` - a new feature
- `fix` - a bug fix
- `build` - changes that affect the build system or external dependencies
- `chore` - changes to the build process or auxiliary tools and libraries
- `ci` - changes to our CI configuration files and scripts
- `docs` - documentation only changes
- `perf` - a code change that improves performance
- `refactor` - a code change that neither fixes a bug nor adds a feature
- `revert` - reverts a previous commit
- `style` - changes that do not affect the meaning of the code
- `test` - adding missing tests or correcting existing tests

The scope is derived from the file path (directory structure).
The description should be clear and concise, written in imperative mood.

---

## Step 1: Get Changed Files

Use `git status --porcelain` to get changes. The format is:
- `M ` = modified (staged)
- ` M` = modified (unstaged)
- `A ` = added (staged)
- `??` = untracked

```bash
# Get all changed files (staged, unstaged, and untracked)
ALL_FILES=$(git status --porcelain | sed 's/^... //' | sort)

# Get only staged files (exclude untracked files starting with ??)
STAGED_FILES=$(git status --porcelain | grep -v '^??' | sed 's/^... //' | sort)
```

If there are no changes, exit early:

```bash
if [ -z "$ALL_FILES" ]; then
    echo "No changes to commit."
    exit 0
fi
```

Display the list of all changed files to the user.

---

## Step 2: Filter Files Based on User Input

Ask the user how they want to handle the files. Options:
- `"commit all"` - include all changed files (staged + unstaged + untracked)
- `"commit staged"` or `"staged only"` - only already staged changes (exclude untracked files)
- `"ignore <file>"` or `"exclude <file>"` - exclude specific files
- `"only <path>"` or `"just <path>"` - include only files matching a path
- `"commit <file1> <file2>"` - commit only specific files

Parse user input and set `CHANGED_FILES` accordingly:

```bash
case "$USER_FILTER" in
    "commit all"|"all")
        CHANGED_FILES="$ALL_FILES"
        ;;
    "commit staged"|"staged"|"staged only")
        CHANGED_FILES="$STAGED_FILES"
        ;;
    "ignore "*|"exclude "*)
        # Remove specified files from ALL_FILES
        IGNORED=$(echo "$USER_FILTER" | sed 's/^[^ ]* //')
        CHANGED_FILES=$(echo "$ALL_FILES" | grep -v "^$IGNORED$")
        ;;
    "only "*|"just "*)
        # Keep only specified files
        WANTED=$(echo "$USER_FILTER" | sed 's/^[^ ]* //')
        CHANGED_FILES=$(echo "$ALL_FILES" | grep "^$WANTED")
        ;;
    *)
        CHANGED_FILES="$ALL_FILES"
        ;;
esac
```

---

## Step 3: Extract Unique Scopes from Changed Files

Extract directory paths as scopes:

```bash
SCOPES=$(for f in $CHANGED_FILES; do dirname "$f" | tr '/' '-'; done | sort -u)
```

If a file is in the root directory, use `.` as the scope.

---

## Step 4: Get Task Summary from Conversation

Read the task completion summary from the conversation context (the task that was just completed).

Populate `CHANGE_DESCRIPTION` with this summary. This description should describe what was accomplished.

---

## Step 5: Auto-detect Commit Type from Keywords

Use keyword matching on the task summary to determine commit type:

```bash
DETECTED_TYPE="chore"  # default

case "$CHANGE_DESCRIPTION" in
    *"feat"*|*"create"*|*"implement"*|*"add"*)
        DETECTED_TYPE="feat";;
    *"fix"*|*"correct"*|*"resolve"*|*"bug"*)
        DETECTED_TYPE="fix";;
    *"system"*|*"dependencies"*|*"dependency"*)
        DETECTED_TYPE="build";;
    *"revert"*|*"previous"*|*"undo"*)
        DETECTED_TYPE="revert";;
    *"jenkins"*|*"CI"*|*"pipeline"*|*"configuration"*|*"script"*)
        DETECTED_TYPE="ci";;
    *"refactor"*|*"restructure"*|*"reorganize"*)
        DETECTED_TYPE="refactor";;
    *"test"*)
        DETECTED_TYPE="test";;
    *"doc"*|*"comment"*|*"documentation"*)
        DETECTED_TYPE="docs";;
    *"style"*|*"format"*|*"lint"*)
        DETECTED_TYPE="style";;
    *"perf"*|*"optimize"*|*"performance"*|*"speed"*)
        DETECTED_TYPE="perf";;
    *"rule"*|*"tool"*|*"auxiliary"*)
        DETECTED_TYPE="chore";;
esac
```

---

## Step 6: Generate Commit Messages for Each Scope

For each unique scope, generate the commit message:

```bash
for SCOPE in $SCOPES; do
    # Find files belonging to this scope
    SCOPE_FILES=""
    for f in $CHANGED_FILES; do
        FILE_SCOPE=$(dirname "$f" | tr '/' '-')
        if [ "$FILE_SCOPE" = "$SCOPE" ]; then
            SCOPE_FILES="$SCOPE_FILES $f"
        fi
    done

    # Generate commit message
    COMMIT_MSG="$DETECTED_TYPE($SCOPE): $CHANGE_DESCRIPTION"
done
```

---

## Step 7: Display Proposed Commits and Request Approval

Display each proposed commit with its files:

```
Proposed commits:
1. $DETECTED_TYPE($SCOPE1): $CHANGE_DESCRIPTION
   Files: file1.c, file2.h

2. $DETECTED_TYPE($SCOPE2): $CHANGE_DESCRIPTION
   Files: file3.c
```

Ask user for approval: `"Approve this commit message?"`

If not approved, allow user to modify the description or cancel.

---

## Step 8: Execute Commits (After Approval)

After user approval, execute the commits:

```bash
for SCOPE in $SCOPES; do
    # Get files for this scope (same logic as Step 6)
    SCOPE_FILES=""
    for f in $CHANGED_FILES; do
        FILE_SCOPE=$(dirname "$f" | tr '/' '-')
        if [ "$FILE_SCOPE" = "$SCOPE" ]; then
            SCOPE_FILES="$SCOPE_FILES $f"
        fi
    done

    if [ -n "$SCOPE_FILES" ]; then
        git add $SCOPE_FILES
        git commit -m "$DETECTED_TYPE($SCOPE): $CHANGE_DESCRIPTION"
    fi
done
```

---

## Examples

### Example 1: Single file commit
- Task: "add user authentication function"
- Files: `src/auth/login.c`
- Output: `feat(src-auth): add user authentication function`

### Example 2: Multiple files in same scope
- Task: "fix voltage reading calibration"
- Files: `lib/adc/adc_functions.c`, `lib/adc/adc_functions.h`
- Output: `fix(lib-adc): fix voltage reading calibration`

### Example 3: Multiple scopes
- Task: "add new ADC calibration feature and update tests"
- Files: `lib/adc/calibration.c`, `lib/adc/calibration.h`, `test/test_calibration/test_calibration.c`
- Output:
  - `feat(lib-adc): add new ADC calibration feature` (commits calibration.c, calibration.h)
  - `test(test-calibration): add new ADC calibration feature` (commits test file)

### Example 4: File filtering - commit staged only
- User says: "commit staged"
- All files: `lib/adc/adc.c` (unstaged), `lib/adc/adc.h` (staged)
- Files after filter: `lib/adc/adc.h` (only staged)
- Output: `fix(lib-adc): <description>`

### Example 5: File filtering - ignore file
- User says: "ignore sdkconfig.test"
- Files before filter: `lib/adc/adc.c`, `sdkconfig.test`
- Files after filter: `lib/adc/adc.c`
- Output: `fix(lib-adc): <description>`
