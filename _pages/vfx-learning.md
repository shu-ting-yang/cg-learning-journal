---
layout: single
title: "VFX Learning"
permalink: /vfx-learning/
author_profile: true
---

<section class="section-hero">
  <p class="section-kicker">Archive</p>
  <h2>Computer Graphics Learning Journey</h2>
  <p class="section-sub">Archive of my computer graphics learning journey, including OpenGL experiments, shaders, and rendering fundamentals.</p>
</section>

{% assign vfx_posts = site.posts | where_exp: 'post', 'post.categories contains "vfx-learning"' | sort: 'date' | reverse %}

<div class="chip-list">
  <a class="chip" href="#graphics">graphics</a>
  <a class="chip" href="#opengl">opengl</a>
  <a class="chip" href="#vfx">vfx</a>
  <a class="chip" href="#shaders">shaders</a>
</div>

<h2>All Archive Posts</h2>
{% if vfx_posts.size > 0 %}
<div class="content-grid content-grid--2">
  {% for post in vfx_posts %}
  <article class="content-card">
    <h3><a href="{{ post.url | relative_url }}">{{ post.title }}</a></h3>
    <p class="meta-line">{{ post.date | date: "%Y-%m-%d" }}</p>
    {% if post.tags %}
    <div class="chip-list">
      {% for tag in post.tags %}
      <span class="chip">{{ tag }}</span>
      {% endfor %}
    </div>
    {% endif %}
  </article>
  {% endfor %}
</div>
{% else %}
<p>No VFX archive posts found.</p>
{% endif %}

{% assign filter_tags = "graphics,opengl,vfx,shaders" | split: "," %}
{% for filter_tag in filter_tags %}
<h2 id="{{ filter_tag }}">{{ filter_tag | capitalize }}</h2>
{% assign filtered_posts = vfx_posts | where_exp: 'post', 'post.tags contains filter_tag' %}
{% if filtered_posts.size > 0 %}
<div class="content-grid content-grid--2">
  {% for post in filtered_posts %}
  <article class="content-card">
    <h3><a href="{{ post.url | relative_url }}">{{ post.title }}</a></h3>
    <p class="meta-line">{{ post.date | date: "%Y-%m-%d" }}</p>
  </article>
  {% endfor %}
</div>
{% else %}
<p>No posts tagged <code>{{ filter_tag }}</code> yet.</p>
{% endif %}
{% endfor %}
