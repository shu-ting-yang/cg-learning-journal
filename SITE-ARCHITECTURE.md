# Site Architecture (Dark-Only Refactor)

## Core Layouts

- `_layouts/default.html`: Global shell (head, header, footer, CSS/JS).
- `_layouts/home.html`: Homepage composition.
- `_layouts/single.html`: Shared content layout for pages, posts, and collections.

## Typography

- Primary UI/body font: `IBM Plex Sans` with `IBM Plex Sans TC` fallback for Traditional Chinese, then platform system sans-serif fonts.
- Monospace font for code/labels: `JetBrains Mono`.

## Reusable Components

- `_includes/site/header.html`: Top navigation.
- `_includes/site/footer.html`: Footer/contact/social links.
- `_includes/site/collection-panel.html`: Reusable section listing cards from any collection (or `posts` filtered by category).

## Styling System

- `assets/css/main.scss`: SCSS entrypoint.
- `_sass/site/_tokens.scss`: Design tokens and color variables.
- `_sass/site/_base.scss`: Base typography, reset, backgrounds.
- `_sass/site/_layout.scss`: Header/footer/shell/home layout.
- `_sass/site/_components.scss`: Cards, panels, prose, tags, buttons.
- `_sass/site/_animations.scss`: Reveal animation and reduced-motion fallback.

## JavaScript

- `assets/js/site.js`: Mobile nav toggle, reveal-on-scroll, footer year.

## Content Routing

- `index.html`: Uses `layout: home`.
- Main hub pages in `_pages/` use `layout: single` + `collection-panel` includes.
- Long-form entries (posts and collection items) keep `layout: single` and are rendered by shared `single` template.

## Main Maintenance Knobs

- Update nav links: `_data/navigation.yml`.
- Update homepage stream cards and featured sections: `_layouts/home.html`.
- Update visual direction/tokens: `_sass/site/_tokens.scss`.
- Update card rendering behavior per stream type: `_includes/site/collection-panel.html`.

## Notes

- Theme toggle is intentionally removed.
- Site is intentionally dark-only (`color-scheme: dark`).
- Legacy Minimal Mistakes templates still exist in the repo, but the active experience now routes through the custom `default/home/single` layouts.
