---
layout: single
title: "Podcast Shows"
permalink: /podcast-shows/
author_profile: true
---

Browse podcast notes grouped by show.

{% assign show_groups = site.podcast_notes | group_by: 'podcast_show' | sort: 'name' %}

{% for group in show_groups %}
{% if group.name and group.name != "" %}
## {{ group.name }}

{% for note in group.items %}
- [{{ note.title }}]({{ note.url | relative_url }}){% if note.episode_number %} ({{ note.episode_number }}){% endif %}
{% endfor %}
{% endif %}

{% endfor %}
