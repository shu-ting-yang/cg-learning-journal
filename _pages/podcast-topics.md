---
layout: single
title: "Podcast Topics"
permalink: /podcast-topics/
author_profile: true
---

Browse podcast notes by topic.

{% for topic in site.data.podcast_topics %}
## {{ topic.name }}
{: #{{ topic.id }}}

{{ topic.description }}

- Filtered view: [Open {{ topic.name }} notes]({{ '/podcast-topics/#' | append: topic.id | relative_url }})

{% assign topic_posts = site.podcast_notes | where_exp: 'note', 'note.topics contains topic.id' | sort: 'date' | reverse %}
{% if topic_posts.size > 0 %}
Recent posts:
{% for note in topic_posts limit:5 %}
- [{{ note.title }}]({{ note.url | relative_url }})
{% endfor %}
{% else %}
No posts yet for this topic.
{% endif %}

{% endfor %}
