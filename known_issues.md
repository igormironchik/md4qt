<!--
	SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
-->

# Known issues

## 1. Lazy HTML

In contrast to CommonMark, `md4qt` has one difference. If in list item first element is HTML
CommonMark doesn't apply rule to lazy continuation lines as this is not a paragraph,
whereas `md4qt` does so. For example.

```md
* <!--
-->
```

In CommonMark will be.

```html
<ul>
<li>
<!--
</li>
</ul>
<p>--&gt;</p>
```

And in `md4qt` it will be.

```html
<ul>
<li>
<!--
-->
</li>
</ul>
```

I know about this difference. And I decided to keep it. But if somebody
will provide compelling reasons to do exactly as in CommonMark in this case, I will
work with this question.

## 2. Content of task list item

GitHub treats everything after task list item as paragraph. I.e.

```md
* [ ] > text
```

Will be.

```html
<ul>
<li class="task-list-item"><input type="checkbox" id="" disabled=""> &gt; text</li>
</ul>
```

Whereas in `md4qt` it will be.

```html
<ul>
<li class="task-list-item"><input type="checkbox" id="" disabled="">

<blockquote><p>text</p></blockquote>
</li>
</ul>
```

Interesting question. And I decided to keep it as is now.
