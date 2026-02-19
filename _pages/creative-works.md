---
layout: single
title: "Creative Works"
permalink: /creative-works/
author_profile: true
---

<section class="section-hero">
  <p class="section-kicker">Portfolio</p>
  <h2>Video, Color, and Photography</h2>
  <p class="section-sub">A curated archive of visual projects, with emphasis on process, toolchain, and visual storytelling decisions.</p>
</section>

{% assign projects = site.creative_works | sort: 'date' | reverse %}
{% if projects.size > 0 %}
  {% assign grouped_projects = projects | group_by: 'project_type' %}
  {% for group in grouped_projects %}
    <h2>{{ group.name | default: "Unspecified" | capitalize }}</h2>
    <div class="content-grid content-grid--2">
      {% for item in group.items %}
      <article class="content-card">
        {% if item.cover_image %}
        <img class="card-cover" src="{{ item.cover_image | relative_url }}" alt="{{ item.title }} cover" />
        {% endif %}
        <h3><a href="{{ item.url | relative_url }}">{{ item.title }}</a></h3>
        <p>{{ item.excerpt | default: "Add a short project summary." }}</p>
        {% if item.tools_used %}
        <p class="meta-line">Tools: {{ item.tools_used | join: ", " }}</p>
        {% endif %}
      </article>
      {% endfor %}
    </div>
  {% endfor %}
{% else %}
<p>No creative projects published yet. Start with <code>_creative_works/TEMPLATE.md</code>.</p>
{% endif %}
