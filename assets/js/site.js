(function () {
  var header = document.querySelector(".site-header");
  var menuToggle = document.querySelector("[data-menu-toggle]");
  var menu = document.querySelector("[data-menu]");
  var yearNode = document.querySelector("[data-current-year]");

  if (yearNode) {
    yearNode.textContent = String(new Date().getFullYear());
  }

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
