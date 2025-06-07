---
layout: single
title: "Learning Journal"
permalink: /learning-log/
math: true
---

Welcome to my daily learning log. This is a raw journal of what I do and learn each day — less polished than my weekly projects, but great for tracking progress and reflection.

{% assign logs = site.pages | where_exp: "page", "page.path contains 'learning-log/'" | sort: "date" %}

{% for log in logs %}
<details>
  <summary><strong>{{ log.title }}</strong></summary>
  <div style="margin-left: 1em;">
    {{ log.content }}
  </div>
</details>
<hr>
{% endfor %}

