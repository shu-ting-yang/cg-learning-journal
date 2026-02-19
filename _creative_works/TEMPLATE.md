---
title: "Project Title"
date: 2025-02-19
project_type: "video"
tools_used:
  - Premiere Pro
  - DaVinci Resolve
tags:
  - editing
  - storytelling
excerpt: "A short description of the creative project."
cover_image: "/assets/images/creative/example-cover.jpg"
youtube_url: ""
instagram_url: ""
gallery:
  - "/assets/images/creative/example-1.jpg"
  - "/assets/images/creative/example-2.jpg"
published: false
---

## Overview

Describe the brief, audience, and creative goal.

## Process

- Concept and references
- Editing and color workflow
- Feedback and revisions

## Final Work

### YouTube Embed

{% raw %}{% if page.youtube_url and page.youtube_url != "" %}
<div class="responsive-embed">
  <iframe src="{{ page.youtube_url }}" title="YouTube video" frameborder="0" allowfullscreen></iframe>
</div>
{% endif %}{% endraw %}

### Instagram Link

{% raw %}{% if page.instagram_url and page.instagram_url != "" %}
[View on Instagram]({{ page.instagram_url }})
{% endif %}{% endraw %}

### Image Gallery

{% raw %}{% if page.gallery %}
<div class="archive">
  {% for image in page.gallery %}
    <img src="{{ image | relative_url }}" alt="{{ page.title }} image {{ forloop.index }}" style="margin-bottom: 1rem;" />
  {% endfor %}
</div>
{% endif %}{% endraw %}

## Learnings

- What worked
- What was challenging
- What you would improve next time
