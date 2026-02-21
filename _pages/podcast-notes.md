---
layout: single
title: "Podcast Notes"
permalink: /podcast-notes/
section_label: "knowledge stream"
description: "Psychology podcast synthesis focused on practical application."
---

<div class="inline-links">
  <a href="{{ '/podcast-topics/' | relative_url }}">Browse by topic</a>
  <a href="{{ '/podcast-shows/' | relative_url }}">Browse by show</a>
</div>

{% if site.data.podcast_topics and site.data.podcast_topics.size > 0 %}
<div class="tag-row">
  {% for topic in site.data.podcast_topics %}
    <a class="tag" href="{{ '/podcast-topics/#' | append: topic.id | relative_url }}">{{ topic.name }}</a>
  {% endfor %}
</div>
{% endif %}

{% include site/collection-panel.html
  title="All Podcast Notes"
  description="Recent entries first."
  source="podcast_notes"
  variant="podcast"
  limit=0
%}
