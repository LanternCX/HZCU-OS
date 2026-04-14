---
name: lab-planning
description: Use when working in this course repository to turn a lab PDF into Markdown and produce a new 作业计划.md that matches an earlier lab's structure, answer style, and screenshot guidance.
license: Proprietary. Repository-local workflow reference.
---

# Lab Planning

## Overview

This skill is for this course repository when a user wants two deliverables from a lab handout:

1. A Markdown version of the lab PDF.
2. A `作业计划.md` that follows the style of an earlier lab directory such as `Lab03/作业计划.md`.

The core rule is: do not stop at extracting text. The final plan must be executable, aligned with the existing repo style, and safe against stale assumptions about the current code state.

## When To Use

Use this when all of these are true:

1. The target is a lab directory like `Lab04/`, `Lab05/`.
2. There is a PDF handout that needs to become Markdown.
3. The user wants a new `作业计划.md`, or wants guidance in the same style as an earlier lab.

Do not use this for generic PDF extraction without a lab plan, or for ordinary code changes unrelated to course handouts.

## Deliverables

The finished output usually includes:

1. `<实验名>.md`
2. `作业计划.md`

Optional supporting output:

1. `实验结果记录.md` template text inside the plan
2. Notes about environment differences if the current machine is not Linux

## Workflow

### 1. Inspect the target lab directory first

Check these before writing anything:

1. The PDF filename in the target lab directory.
2. The code or sample files in the lab subdirectory.
3. The reference lab directory the user wants to imitate, usually the previous lab.

Read these files early:

1. `LabXX/<实验名>.pdf`
2. `LabYY/作业计划.md`
3. Any existing `LabYY/<实验名>.md`
4. The sample source files under the new lab directory

## 2. Convert the PDF to Markdown notes

Preferred extraction order:

1. Use `pdftotext -layout` if available.
2. If `pdftotext` is unavailable, use `pypdf` to extract text.
3. If the PDF is scanned and text extraction is poor, note that OCR may be needed.

Minimum acceptable conversion process:

1. Extract page text.
2. Rebuild headings manually.
3. Preserve code blocks, questions, and numbered experiment steps.
4. Save the cleaned result as Markdown in the lab directory.

The Markdown file is not just raw text output. It should be cleaned into readable sections such as:

1. 实验目的
2. 实验内容
3. 实验步骤
4. 例程或编程题

## 3. Reverse-engineer the reference plan style

Before drafting the new plan, identify the reference file's patterns.

Look for:

1. Title format, usually `# LabXX 作业计划`
2. Task-based structure
3. Standard section names under each task
4. How answers are written
5. How screenshot requirements are described

In this repository, the important style markers are usually:

1. `### 执行命令`
2. `### 什么时候截图` or `### 要截图的内容`
3. `### 报告放置位置`
4. `### 本 task 要回答的问题`

## 4. Verify actual code state before writing the plan

This is critical.

Do not write instructions like “先修改代码” unless the current files still need that step.

Before writing any task that mentions editing source files, confirm:

1. Whether the repository already contains the required fixes.
2. Whether the experiment intentionally asks the student to toggle lines to observe different phenomena.

Use this rule:

1. If a code change was already applied to make the sample runnable, the plan should treat that as current state, not as future work for the user.
2. If a code change is part of the experiment itself, keep it as a task.

Examples:

1. Adding missing headers so code compiles: treat as already done if the file is already fixed.
2. Commenting out `sleep()` or enabling mutex lines to compare phenomena: keep as explicit experiment steps.

## 5. Draft the new 作业计划.md as an execution guide, not a summary

Each task should be something the user can follow directly.

At minimum, each task should answer four questions:

1. What command do I run?
2. What should I observe?
3. When exactly should I take the screenshot?
4. How do I answer the report question?

The plan must not merely restate the PDF. It must turn the PDF into an operational sequence.

## 6. Write screenshot guidance as timing, not vague content

This is a common failure point.

Bad style:

1. “截图程序运行结果”

Better style:

1. “执行完编译命令且没有报错后，先截一张编译成功图。”
2. “运行结果完整显示后再截图。”
3. “如果是多次运行对比，尽量把两次以上结果放在同一张图里。”

When writing screenshot guidance, always specify:

1. After which command to screenshot
2. What must be visible in that screenshot
3. Whether a code screenshot is also required

## 7. Match the answer style of the reference lab

Answers in `作业计划.md` should be written in the same tone and density as the earlier lab.

In this repository, that usually means:

1. Each question gets a direct answer under it.
2. The answer is longer than a fragment, but still plain and readable.
3. The answer explains the phenomenon and the reason in one short paragraph.

Target pattern:

1. Start with “因为……” or an equivalent direct explanation.
2. Explain the observed phenomenon first.
3. Then explain why it happens.

Avoid these mistakes:

1. Answers that are too short to submit directly.
2. Answers that sound more casual than the reference plan.
3. Answers that describe implementation details instead of the experiment phenomenon.

## 8. Add environment notes only when necessary

If the lab handout is Linux-oriented but the current machine differs, add a brief note in the plan.

Use this format:

1. State what the handout expects.
2. State what differs on the current machine.
3. Give the current-machine substitute only if it helps the user proceed.

Do not let environment notes take over the plan.

## 9. Final consistency check before saving

Check all of these:

1. The PDF-derived Markdown reflects the actual handout title and sections.
2. The new `作业计划.md` matches the reference structure.
3. No task tells the user to perform a fix that is already present.
4. Screenshot timing is explicit.
5. Report answers are written in the same style level as the reference lab.
6. Task names and record-file text do not mention outdated step names.

## Quick Reference

| Goal | What to do |
|------|------------|
| Extract PDF text | Try `pdftotext -layout`, else use `pypdf` |
| Build lab Markdown | Clean text into headings, lists, and code blocks |
| Match old plan style | Read previous `作业计划.md` first |
| Avoid stale instructions | Check current code state before describing tasks |
| Improve screenshot guidance | Say when to screenshot, not only what to screenshot |
| Improve report answers | Match the earlier lab's sentence length and tone |

## Example Command Snippets

### Extract PDF with pypdf

```bash
python3 - <<'PY'
from pypdf import PdfReader
reader = PdfReader('实验五 线程的创建和管理.pdf')
for i, page in enumerate(reader.pages, 1):
    print(f'===== PAGE {i} =====')
    print(page.extract_text() or '')
PY
```

### Files to read before drafting

```text
Lab04/实验五 线程的创建和管理.pdf
Lab03/作业计划.md
Lab03/对应实验 Markdown
Lab04/thread/*.c
```

## Common Mistakes

1. Converting the PDF into plain notes, but not into an actionable plan.
2. Copying the old plan's headings while missing its answer style.
3. Telling the user to “modify the code” even though the repository already contains the fix.
4. Writing screenshot guidance that does not tell the user when to capture the image.
5. Leaving task names or record-file content in an outdated state after the plan evolves.

## Done Criteria

This skill has been applied correctly when:

1. The target lab directory contains a cleaned Markdown version of the handout.
2. The new `作业计划.md` can be followed task by task without guessing.
3. The screenshot instructions are precise enough that the user does not need to ask where to screenshot.
4. The report answers read like the earlier lab plan, not like brief ad hoc chat replies.
