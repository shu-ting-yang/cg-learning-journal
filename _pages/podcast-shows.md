---
layout: single
title: "Podcast Shows"
permalink: /podcast-shows/
author_profile: true
---

Browse podcast notes grouped by show.

{% assign valid_notes = site.podcast_notes | where_exp: 'note', 'note.podcast_show and note.podcast_show != ""' %}
{% assign show_groups = valid_notes | group_by: 'podcast_show' | sort: 'name' %}

{% for group in show_groups %}
## {{ group.name }}

{% for note in group.items %}
- [{{ note.title }}]({{ note.url | relative_url }}){% if note.episode_number %} ({{ note.episode_number }}){% endif %}
{% endfor %}

{% endfor %}
