<!--
	SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
-->

# 1. Autolinks

In this library autolinks are checked by `QUrl` for validity. CommonMark is not so strict,
his rules are much more simpler for it. As example.

```md
<made-up-scheme://foo,bar>
```

In CommonMark will be a valid autolink, whereas in `md4qt` won't be.

**Interested in this question may start a discussion with me [here](https://invent.kde.org/libraries/md4qt/-/issues/7).**
