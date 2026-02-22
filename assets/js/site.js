(function () {
  var root = document.documentElement;
  var THEME_KEY = "theme";
  var header = document.querySelector(".site-header");
  var menuToggle = document.querySelector("[data-menu-toggle]");
  var menu = document.querySelector("[data-menu]");
  var themeToggle = document.querySelector("[data-theme-toggle]");
  var yearNode = document.querySelector("[data-current-year]");

  if (yearNode) {
    yearNode.textContent = String(new Date().getFullYear());
  }

  function getStoredTheme() {
    try {
      return localStorage.getItem(THEME_KEY);
    } catch (_err) {
      return null;
    }
  }

  function setStoredTheme(theme) {
    try {
      localStorage.setItem(THEME_KEY, theme);
    } catch (_err) {
      // Ignore storage errors (private mode, blocked storage).
    }
  }

  function applyTheme(theme) {
    if (theme === "dark") {
      root.setAttribute("data-theme", "dark");
    } else {
      root.removeAttribute("data-theme");
    }
    updateThemeToggle();
  }

  function updateThemeToggle() {
    if (!themeToggle) {
      return;
    }

    var isDark = root.getAttribute("data-theme") === "dark";
    var label = isDark ? "Switch to light theme" : "Switch to dark theme";
    var icon = isDark ? "☀" : "☾";
    var iconNode = themeToggle.querySelector("span");

    themeToggle.setAttribute("aria-label", label);
    themeToggle.setAttribute("title", label);
    if (iconNode) {
      iconNode.textContent = icon;
    } else {
      themeToggle.textContent = icon;
    }
  }

  applyTheme(getStoredTheme() === "dark" ? "dark" : "light");

  function closeMenu() {
    if (!header || !menuToggle) {
      return;
    }
    header.classList.remove("is-open");
    menuToggle.setAttribute("aria-expanded", "false");
  }

  if (header && menuToggle && menu) {
    menuToggle.addEventListener("click", function () {
      var isOpen = header.classList.toggle("is-open");
      menuToggle.setAttribute("aria-expanded", isOpen ? "true" : "false");
    });

    menu.addEventListener("click", function (event) {
      if (event.target && event.target.tagName === "A") {
        closeMenu();
      }
    });

    document.addEventListener("click", function (event) {
      if (!header.contains(event.target)) {
        closeMenu();
      }
    });

    document.addEventListener("keydown", function (event) {
      if (event.key === "Escape") {
        closeMenu();
      }
    });
  }

  if (themeToggle) {
    themeToggle.addEventListener("click", function () {
      var isDark = root.getAttribute("data-theme") === "dark";
      var nextTheme = isDark ? "light" : "dark";
      applyTheme(nextTheme);
      setStoredTheme(nextTheme);
    });
  }

  var revealItems = document.querySelectorAll("[data-reveal]");
  if (!revealItems.length) {
    return;
  }

  if (!("IntersectionObserver" in window)) {
    revealItems.forEach(function (item) {
      item.classList.add("is-visible");
    });
    return;
  }

  var observer = new IntersectionObserver(
    function (entries, currentObserver) {
      entries.forEach(function (entry) {
        if (!entry.isIntersecting) {
          return;
        }
        entry.target.classList.add("is-visible");
        currentObserver.unobserve(entry.target);
      });
    },
    {
      rootMargin: "0px 0px -12% 0px",
      threshold: 0.1,
    }
  );

  revealItems.forEach(function (item) {
    observer.observe(item);
  });
})();
