# uclip

`uclip` is a simple Windows console utility to copy UTF-8 text to clipboard.

It makes it convenient for use under Cygwin or Git Bash.

If your text is not in UTF-8, convert it first with something like `iconv`.

When using `cmd.exe`, stick with `clip.exe` that ships with Windows.  Just remember to work under `chcp 65001` so that Unicode is handled properly.

## Usage reference

1\. cmd.exe

```
chcp 65001
echo ✓ | clip
```

2\. Cygwin

```
echo ✓ | uclip
```

3\. Git Bash

```
echo ✓ | uclip
```
