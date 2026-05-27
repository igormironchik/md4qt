<!--
	SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
-->

# 1. Autolinks

By default this library checks autolinks with `QUrl` for validity. CommonMark is not so strict,
and its rules are much simpler for it. As example.

```md
<made-up-scheme://foo,bar>
```

In CommonMark validation mode this is a valid autolink, whereas the default `QUrl` validation
mode preserves the stricter historical behavior.

In a local microbenchmark of URL validation only, using the same mixed set of 12 autolink
destinations for both modes, CommonMark validation was about 1.45x faster than `QUrl`
validation. Real document parsing speedups will depend on how often autolinks appear.

**Interested in this question may start a discussion with me [here](https://invent.kde.org/libraries/md4qt/-/issues/7).**
