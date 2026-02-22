(function () {
  "use strict";

  var scrollFrame = document.getElementById("scroll-frame");
  var loader = document.getElementById("mh-loader");
  var loaderBar = document.getElementById("mh-loader-bar");
  var cursorDot = document.getElementById("cursor-dot");
  var navHb = document.getElementById("nav-hb");
  var mobileNav = document.getElementById("mobile-nav");
  var navTargets = document.querySelectorAll("[data-target]");
  var homeBgImage = document.getElementById("home-bg-image");
  var workTrackWrap = document.getElementById("work-track-wrap");
  var workDetail = document.getElementById("work-detail");
  var workDetailClose = document.getElementById("work-detail-close");
  var workCards = document.querySelectorAll(".work-card");
  var revealEls = document.querySelectorAll(".reveal");
  var yearEl = document.getElementById("footer-year");

  if (!scrollFrame) return;

  function setYear() {
    if (yearEl) yearEl.textContent = String(new Date().getFullYear());
  }

  function loadAnimation() {
    var progress = 0;
    var timer = setInterval(function () {
      progress += Math.floor(Math.random() * 16) + 8;
      if (progress > 100) progress = 100;
      if (loaderBar) loaderBar.style.width = progress + "%";
      if (progress >= 100) {
        clearInterval(timer);
        setTimeout(function () {
          if (loader) loader.classList.add("is-hidden");
        }, 240);
      }
    }, 120);
  }

  function smoothTo(targetId) {
    var section = document.getElementById(targetId);
    if (!section) return;
    var offset = window.innerHeight * 0.14;
    var top = section.offsetTop - offset;
    scrollFrame.scrollTo({ top: top, behavior: "smooth" });
  }

  function bindNav() {
    navTargets.forEach(function (node) {
      node.addEventListener("click", function (e) {
        e.preventDefault();
        var id = node.getAttribute("data-target");
        if (id) smoothTo(id);
        if (mobileNav) mobileNav.classList.remove("is-open");
        if (navHb) navHb.classList.remove("is-open");
      });
    });

    if (navHb && mobileNav) {
      navHb.addEventListener("click", function () {
        navHb.classList.toggle("is-open");
        mobileNav.classList.toggle("is-open");
      });
    }
  }

  function bindCursor() {
    if (!cursorDot || window.matchMedia("(pointer: coarse)").matches || window.innerWidth <= 750) {
      if (cursorDot) cursorDot.classList.add("is-hidden");
      return;
    }

    var current = { x: -120, y: -120 };
    var target = { x: -120, y: -120 };

    function loop() {
      current.x += (target.x - current.x) * 0.22;
      current.y += (target.y - current.y) * 0.22;
      cursorDot.style.transform = "translate3d(" + current.x + "px," + current.y + "px,0)";
      requestAnimationFrame(loop);
    }

    document.addEventListener("mousemove", function (e) {
      target.x = e.clientX - cursorDot.offsetWidth / 2;
      target.y = e.clientY - cursorDot.offsetHeight / 2;

      var t = e.target;
      if (t && (t.closest("a") || t.closest("button") || t.classList.contains("clickable"))) {
        cursorDot.classList.add("is-hover");
      } else {
        cursorDot.classList.remove("is-hover");
      }
    });

    loop();
  }

  function bindParallax() {
    if (!homeBgImage) return;
    scrollFrame.addEventListener("scroll", function () {
      var y = scrollFrame.scrollTop;
      homeBgImage.style.transform = "translate3d(0," + y * 0.09 + "px,0) scale(1.06)";
    });
  }

  function bindWorkDrag() {
    if (!workTrackWrap) return;
    var isDown = false;
    var startX = 0;
    var scrollStart = 0;

    workTrackWrap.addEventListener("mousedown", function (e) {
      isDown = true;
      workTrackWrap.classList.add("is-dragging");
      startX = e.pageX;
      scrollStart = workTrackWrap.scrollLeft;
    });

    window.addEventListener("mouseup", function () {
      isDown = false;
      if (workTrackWrap) workTrackWrap.classList.remove("is-dragging");
    });

    workTrackWrap.addEventListener("mousemove", function (e) {
      if (!isDown) return;
      e.preventDefault();
      var dx = e.pageX - startX;
      workTrackWrap.scrollLeft = scrollStart - dx * 1.5;
    });
  }

  function bindWorkDetail() {
    if (!workDetail) return;

    function closeDetail() {
      workDetail.classList.remove("is-open");
      document.body.style.overflow = "";
    }

    if (workDetailClose) {
      workDetailClose.addEventListener("click", closeDetail);
    }

    workDetail.addEventListener("click", function (e) {
      if (e.target === workDetail) closeDetail();
    });

    workCards.forEach(function (card) {
      var btn = card.querySelector(".work-btn");
      if (!btn) return;
      btn.addEventListener("click", function () {
        var title = card.getAttribute("data-title") || "";
        var summary = card.getAttribute("data-summary") || "";
        var description = card.getAttribute("data-description") || "";
        var roles = card.getAttribute("data-roles") || "";
        var link = card.getAttribute("data-link") || "";
        var linkText = card.getAttribute("data-link-text") || "Visit";

        var titleEl = document.getElementById("detail-title");
        var summaryEl = document.getElementById("detail-summary");
        var descEl = document.getElementById("detail-description");
        var rolesEl = document.getElementById("detail-roles");
        var linkEl = document.getElementById("detail-link");

        if (titleEl) titleEl.textContent = title;
        if (summaryEl) summaryEl.textContent = summary;
        if (descEl) descEl.textContent = description;
        if (rolesEl) {
          rolesEl.innerHTML = "";
          roles.split("|").filter(Boolean).forEach(function (r) {
            var li = document.createElement("li");
            li.textContent = "+ " + r.trim();
            rolesEl.appendChild(li);
          });
        }
        if (linkEl) {
          if (link) {
            linkEl.textContent = linkText;
            linkEl.setAttribute("href", link);
            linkEl.style.display = "inline-block";
          } else {
            linkEl.style.display = "none";
          }
        }

        workDetail.classList.add("is-open");
        document.body.style.overflow = "hidden";
      });
    });
  }

  function bindReveal() {
    if (!("IntersectionObserver" in window)) {
      revealEls.forEach(function (el) { el.classList.add("is-visible"); });
      return;
    }

    var observer = new IntersectionObserver(function (entries) {
      entries.forEach(function (entry) {
        if (entry.isIntersecting) {
          entry.target.classList.add("is-visible");
          observer.unobserve(entry.target);
        }
      });
    }, { root: scrollFrame, threshold: 0.16 });

    revealEls.forEach(function (el) { observer.observe(el); });
  }

  window.addEventListener("load", function () {
    setYear();
    loadAnimation();
    bindNav();
    bindCursor();
    bindParallax();
    bindWorkDrag();
    bindWorkDetail();
    bindReveal();
  });
})();
