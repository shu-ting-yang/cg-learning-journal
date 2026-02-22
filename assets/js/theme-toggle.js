// Theme + layout controls with daypart color mood
(function () {
  var root = document.documentElement;
  var THEME_KEY = "theme";
  var LAYOUT_KEY = "layout_mode";

  function storedTheme() {
    return localStorage.getItem(THEME_KEY) || "default";
  }

  function storedLayout() {
    return localStorage.getItem(LAYOUT_KEY) || "magazine";
  }

  function currentDaypart() {
    var hour = new Date().getHours();
    if (hour >= 5 && hour < 11) return "morning";
    if (hour >= 11 && hour < 17) return "day";
    if (hour >= 17 && hour < 21) return "evening";
    return "night";
  }

  function applyTheme(theme) {
    root.setAttribute("data-theme", theme);
  }

  function applyLayout(layout) {
    root.setAttribute("data-layout", layout);
  }

  function applyDaypart() {
    root.setAttribute("data-daypart", currentDaypart());
  }

  function updateButtons() {
    var themeToggle = document.getElementById("theme-toggle");
    var layoutToggle = document.getElementById("layout-toggle");
    var isDark = root.getAttribute("data-theme") === "dark";
    var layout = root.getAttribute("data-layout");

    if (themeToggle) {
      themeToggle.textContent = isDark ? "☀" : "☾";
      themeToggle.setAttribute(
        "aria-label",
        isDark ? "Switch to light theme" : "Switch to dark theme"
      );
    }

    if (layoutToggle) {
      var isMagazine = layout === "magazine";
      layoutToggle.textContent = isMagazine ? "▦" : "◫";
      layoutToggle.setAttribute(
        "aria-label",
        isMagazine ? "Switch to compact layout" : "Switch to magazine layout"
      );
      layoutToggle.setAttribute(
        "title",
        isMagazine ? "Magazine layout" : "Compact layout"
      );
    }
  }

  function toggleTheme() {
    var isDark = root.getAttribute("data-theme") === "dark";
    var next = isDark ? "default" : "dark";
    applyTheme(next);
    localStorage.setItem(THEME_KEY, next);
    updateButtons();
  }

  function toggleLayout() {
    var layout = root.getAttribute("data-layout");
    var next = layout === "compact" ? "magazine" : "compact";
    applyLayout(next);
    localStorage.setItem(LAYOUT_KEY, next);
    updateButtons();
  }

  document.addEventListener("DOMContentLoaded", function () {
    applyTheme(storedTheme());
    applyLayout(storedLayout());
    applyDaypart();
    updateButtons();

    var themeToggle = document.getElementById("theme-toggle");
    var layoutToggle = document.getElementById("layout-toggle");

    if (themeToggle) themeToggle.addEventListener("click", toggleTheme);
    if (layoutToggle) layoutToggle.addEventListener("click", toggleLayout);

    // Re-sync mood colors in long sessions.
    setInterval(applyDaypart, 5 * 60 * 1000);
  });
})();
