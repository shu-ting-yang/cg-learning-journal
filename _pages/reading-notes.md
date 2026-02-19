---
layout: single
title: "Reading Notes"
permalink: /reading-notes/
author_profile: true
---

<section class="section-hero">
  <p class="section-kicker">Book Notes</p>
  <h2>Applied Psychology Reading</h2>
  <p class="section-sub">Structured notes from psychology books, with concepts, quotes, and practical applications for life and creative work.</p>
</section>

{% assign notes = site.reading_notes | sort: 'date' | reverse %}
{% if notes.size > 0 %}
<div class="content-grid content-grid--2">
  {% for note in notes %}
  <article class="content-card reading-card">
    {% if note.cover_image %}
    <img class="reading-cover" src="{{ note.cover_image | relative_url }}" alt="{{ note.title }} cover" />
    {% else %}
    <div class="reading-cover reading-cover--placeholder">No Cover</div>
    {% endif %}
    <div>
      <h3><a href="{{ note.url | relative_url }}">{{ note.title }}</a></h3>
      <p class="meta-line">{{ note.author | default: "Author TBD" }}{% if note.rating %} · Rating {{ note.rating }}/5{% endif %}</p>
      <p>{{ note.excerpt | default: "Add an excerpt to summarize the book's core contribution." }}</p>
      {% if note.tags %}
      <div class="chip-list">
        {% for tag in note.tags %}
        <span class="chip">{{ tag }}</span>
        {% endfor %}
      </div>
      {% endif %}
    </div>
  </article>
  {% endfor %}
</div>
{% else %}
<p>No reading notes published yet. Start with <code>_reading_notes/TEMPLATE.md</code>.</p>
{% endif %}
