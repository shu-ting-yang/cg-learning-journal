# Deployment Notes

## 1. What was changed

- Repositioned the site as a personal brand website focused on psychology, creativity, and growth.
- Updated global site settings in `_config.yml`:
  - New title and description
  - Timezone set to `Europe/London`
  - New collections with output enabled:
    - `podcast_notes`
    - `creative_works`
    - `reading_notes`
  - Global permalink pattern set to `/:collection/:title/`
  - Defaults for all three collections (`layout: single`, `author_profile: true`)
  - Post defaults keep posts in VFX archive with `categories: [vfx-learning]`
- Replaced navigation in `_data/navigation.yml` with:
  - Home, Podcast Notes, Creative Works, Reading Notes, VFX Learning, About
- Added collection folders and starter templates:
  - `_podcast_notes/TEMPLATE.md`
  - `_podcast_notes/2025-02-example.md`
  - `_creative_works/TEMPLATE.md`
  - `_reading_notes/TEMPLATE.md`
- Added section landing pages in `_pages/`:
  - `podcast-notes.md`
  - `creative-works.md`
  - `reading-notes.md`
  - `vfx-learning.md`
  - `about.md`
- Added podcast taxonomy pages:
  - `_pages/podcast-topics.md`
  - `_pages/podcast-shows.md`
- Added taxonomy data files:
  - `_data/podcast_topics.yml`
  - `_data/podcast_shows.yml`
- Rebuilt homepage content in `index.html` with:
  - Hero intro
  - Featured latest 3 podcast notes
  - Three-column section grid with View All links
- Normalized all existing files in `_posts/` with valid YAML front matter and `categories: [vfx-learning]`.

## 2. How to add new content

### Podcast Notes

1. Copy `_podcast_notes/TEMPLATE.md`.
2. Rename file to a date-stamped slug, for example: `2026-02-emotional-regulation.md`.
3. Fill in front matter fields:
   - `title`, `date`, `podcast_show`, `episode_number`, `topics`, `tags`, `excerpt`
4. Valid topic IDs are in `_data/podcast_topics.yml`:
   - `emotions`, `relationships`, `cognition`, `growth`

### Creative Works

1. Copy `_creative_works/TEMPLATE.md`.
2. Set `project_type` to `video` or `photography`.
3. Add `tools_used`, `tags`, and optional media fields:
   - `youtube_url`
   - `instagram_url`
   - `gallery` image paths

### Reading Notes

1. Copy `_reading_notes/TEMPLATE.md`.
2. Fill in `title`, `author`, `date`, `rating`, `tags`, `excerpt`.
3. Optional: add `cover_image` path to display a book cover.

## 3. How to customize the About page

- Edit `_pages/about.md`.
- Replace placeholder paragraphs in these sections:
  - Psychology Podcast Notes
  - Creative Works
  - Reading Notes
  - VFX Learning Archive
- Author sidebar metadata is controlled in `_config.yml` under `author:`.

## 4. Manual steps and content assets

- Instagram embeds require valid public post/reel URLs in `instagram_url`.
- YouTube embeds require an embeddable URL in `youtube_url`.
- Add project and book images under `assets/images/` and reference them with site-relative paths.
- If you change `_config.yml`, restart `jekyll serve` because config is not hot-reloaded.
- After adding new content, verify:
  - Navigation routes
  - Collection pages render entries
  - Topic/show taxonomy pages include new podcast notes
