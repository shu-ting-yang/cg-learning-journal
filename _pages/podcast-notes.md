---
layout: single
title: "Podcast Notes"
permalink: /podcast-notes/
author_profile: true
---

<section class="section-hero">
  <p class="section-kicker">Main Feature</p>
  <h2>Psychology Podcast Synthesis</h2>
  <p class="section-sub">These notes capture core ideas, memorable quotes, and practical takeaways from psychology podcasts. The goal is to turn listening into applied learning.</p>
</section>

<div class="quick-links">
  <a href="{{ '/podcast-topics/' | relative_url }}">Browse by Topic</a>
  <a href="{{ '/podcast-shows/' | relative_url }}">Browse by Show</a>
</div>

<h2>Topic Lens</h2>
<div class="chip-list">
  {% for topic in site.data.podcast_topics %}
  <a class="chip" href="{{ '/podcast-topics/#' | append: topic.id | relative_url }}">{{ topic.name }}</a>
  {% endfor %}
</div>

<h2>Latest Notes</h2>
{% assign notes = site.podcast_notes | sort: 'date' | reverse %}
{% if notes.size > 0 %}
<div class="content-grid content-grid--3">
  {% for note in notes %}
  <article class="content-card">
    <h3><a href="{{ note.url | relative_url }}">{{ note.title }}</a></h3>
    <p>{{ note.excerpt | default: "Add an excerpt to improve scannability." }}</p>
    <p class="meta-line"><strong>{{ note.podcast_show | default: "Show TBD" }}</strong>{% if note.episode_number %} · {{ note.episode_number }}{% endif %}</p>
    {% if note.topics %}
    <div class="chip-list">
      {% for topic in note.topics %}
      <span class="chip">{{ topic }}</span>
      {% endfor %}
    </div>
    {% endif %}
  </article>
  {% endfor %}
</div>
{% else %}
<p>No podcast notes published yet. Start with <code>_podcast_notes/TEMPLATE.md</code>.</p>
{% endif %}
