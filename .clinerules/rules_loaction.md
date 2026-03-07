# Cline Rules Location

Rules for placing and organizing cline rule files in the repository.

## filters:
  # Match any .md files
  - type: file_extension
    pattern: "\\.md$"
  # Match file creation events
  - type: event
    pattern: "file_create"

## actions:
  - type: reject
    conditions:
      - pattern: "^(?!\\.\\/\\.clinerules\\/.*\\.md$)"
        message: "Cline rule files (.md) must be placed in the .clinerules directory"

  - type: suggest
    message: |
      When creating Cline rules:

      1. Always place rule files in PROJECT_ROOT/.clinerules/:
         ```
         .clinerules/
         ├── your-rule-name.md
         ├── another-rule.md
         └── ...
         ```

      2. Follow the naming convention:
         - Use kebab-case for filenames
         - Always use .md extension
         - Make names descriptive of the rule's purpose

      3. Directory structure:
         ```
         PROJECT_ROOT/
         ├── .clinerules/
         │    ├── your-rule-name.md
         │    └── ...
         └── ...
         ```

      4. Never place rule files:
         - In the project root
         - In subdirectories outside .clinerules
         - In any other location

## Examples:
  - input: |
      # Bad: Rule file in wrong location
      rules/my-rule.md
      my-rule.md
      .rules/my-rule.md

      # Good: Rule file in correct location
      .clinerules/my-rule.md
    output: "Correctly placed rule file"

## metadata:
  priority: high
  version: 1.0