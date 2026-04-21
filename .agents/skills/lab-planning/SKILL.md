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

Two repository-specific constraints are critical:

1. When converting the PDF to Markdown, preserve the full original content. Reformat it into clean Markdown, but do not delete, summarize, or silently rewrite away parts of the handout.
2. When writing `作业计划.md`, the questions and answer prompts must align directly with the original lab handout wording. Do not replace the handout's questions with your own rephrased discussion points.

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

Formatting rule for this repository:

1. The Markdown conversion should be a formatting cleanup, not a content rewrite.
2. Keep the handout complete from beginning to end.
3. It is fine to add Markdown headings, lists, fenced code blocks, and spacing.
4. Do not trim sections, compress question blocks, or replace original wording with summaries.

The Markdown file is not just raw text output. It should be cleaned into readable sections such as:

1. 实验目的
2. 实验内容
3. 实验步骤
4. 例程或编程题

Important nuance:

1. “Cleaned into readable sections” means Markdown formatting only.
2. It does not mean deleting original material or inventing a new outline that hides parts of the source PDF.

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
3. `### 本部分产出`
4. `### 本部分要回答的问题`

Current preferred plan style in this repository:

1. The overall section order should follow the generated lab Markdown, not an unrelated generic task list.
2. Keep the same top-level flow as the handout, for example `实验目的和要求` -> `实验内容` -> `实验步骤` -> each numbered subsection.
3. Under each actionable subsection, use compact blocks such as `### 执行命令`, `### 什么时候截图`, `### 本部分产出`, and `### 本部分要回答的问题`.
4. Do not add `报告放置位置` unless the user explicitly asks for it.

Current preferred execution format for `作业计划.md`:

1. The whole document should read as a serial step-by-step guide that the user can follow from top to bottom without re-planning.
2. Only four step types should appear in the plan: `运行命令`, `修改文件`, `截图`, `回答问题`.
3. These four step types may repeat inside the same task. For example: `运行命令` -> `截图` -> `修改文件` -> `运行命令` -> `截图` -> `回答问题`.
4. Do not force every task into one fixed block layout if the real experiment flow is iterative.
5. When a code change is required, `修改文件` must say exactly which file to edit and which statements, sections, or initialization values need to be changed.
6. If the experiment requires a larger code rewrite rather than a small comment toggle or a couple of line edits, provide the full replacement code directly in the plan instead of leaving the user to infer the final program.

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

At the same time, keep this boundary:

1. The operational sequence must stay anchored to the handout's original section order.
2. Do not replace the handout's own questions with newly invented ones just because they sound clearer.
3. If the handout says “理解……”, “分析……”, “观察……”, or “请修改例程”, preserve that as the prompt being answered.
4. If the handout's real workflow is run -> observe -> modify -> rerun, the plan should preserve that sequence explicitly rather than collapsing it into a summary.

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

1. Use direct, natural report language.
2. Explain the observed phenomenon first.
3. Then explain why it happens.

Avoid these mistakes:

1. Answers that are too short to submit directly.
2. Answers that sound more casual than the reference plan.
3. Answers that describe implementation details instead of the experiment phenomenon.
4. Answers that respond to your own rewritten question instead of the lab handout's actual prompt.
5. Hiding required file edits behind vague wording such as “请修改例程” without saying what file and where to change it.
6. Converting a sequential experiment into a grouped checklist that the user cannot follow linearly.
7. Using stiff filler phrasing such as repeated “因为……所以……” openings that reads unnaturally in a report.
8. For large code changes, describing the intent only and making the user reconstruct the full code themselves.

Question-alignment rule:

1. For each experiment subsection, copy or closely preserve the handout's original prompt before answering it.
2. If the handout lists two or three points after “并思考下述问题”, the plan should answer those same two or three points in the same order.
3. You may clarify the answer, but you should not replace the original prompt with a looser paraphrase such as “这里体现了什么现象？” unless the handout itself asks that.

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
2. The PDF-derived Markdown preserves the full handout content and only cleans up formatting.
3. The new `作业计划.md` matches the reference structure.
4. The plan section order follows the handout's own structure.
5. The answer prompts in `作业计划.md` match the handout's original questions or directives.
6. The plan can be executed top-to-bottom as a serial workflow.
7. Only the allowed step types are used: `运行命令`, `修改文件`, `截图`, `回答问题`.
8. Every required code change names the file and the concrete place to edit.
9. Large code changes include full replacement code in the plan.
10. No task tells the user to perform a fix that is already present.
11. Screenshot timing is explicit.
12. Report answers are written in the same style level as the reference lab.
13. Task names and record-file text do not mention outdated step names.

## Quick Reference

| Goal | What to do |
|------|------------|
| Extract PDF text | Try `pdftotext -layout`, else use `pypdf` |
| Build lab Markdown | Clean formatting only; preserve all original content |
| Match old plan style | Read previous `作业计划.md` first |
| Avoid stale instructions | Check current code state before describing tasks |
| Improve screenshot guidance | Say when to screenshot, not only what to screenshot |
| Improve report answers | Match the earlier lab's sentence length and tone |
| Keep prompts aligned | Answer the handout's actual questions in order |
| Keep steps followable | Make the plan executable line by line |

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
2. Rewriting the PDF so aggressively that original content, wording, or question prompts are lost.
3. Copying the old plan's headings while missing its answer style.
4. Answering your own paraphrased questions instead of the lab handout's actual questions.
5. Telling the user to “modify the code” even though the repository already contains the fix.
6. Writing screenshot guidance that does not tell the user when to capture the image.
7. Leaving task names or record-file content in an outdated state after the plan evolves.
8. Using a rigid section template that prevents the user from following the real run-modify-rerun order of the experiment.

## Done Criteria

This skill has been applied correctly when:

1. The target lab directory contains a cleaned Markdown version of the handout with the full original content preserved.
2. The new `作业计划.md` follows the handout's section order and can be followed subsection by subsection without guessing.
3. The screenshot instructions are precise enough that the user does not need to ask where to screenshot.
4. The answer prompts in `作业计划.md` match the handout's original questions or directives.
5. The report answers read like the earlier lab plan, not like brief ad hoc chat replies.
6. The plan uses only `运行命令`, `修改文件`, `截图`, and `回答问题` as step types, and these steps appear in the actual execution order.
