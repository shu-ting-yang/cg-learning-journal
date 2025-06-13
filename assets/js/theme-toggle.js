// Enhanced Theme toggle functionality
document.addEventListener('DOMContentLoaded', function() {
    console.log('Theme toggle script loaded');
    
    // Create theme toggle button
    function createThemeToggle() {
        // Remove existing toggle if present
        const existingToggle = document.getElementById('theme-toggle');
        if (existingToggle) {
            existingToggle.remove();
        }
        
        const toggleButton = document.createElement('button');
        toggleButton.id = 'theme-toggle';
        toggleButton.setAttribute('aria-label', 'Toggle dark mode');
        toggleButton.setAttribute('title', 'Toggle dark/light theme');
        
        // Try to add to masthead first, fallback to body
        const masthead = document.querySelector('.masthead');
        if (masthead) {
            masthead.appendChild(toggleButton);
        } else {
            document.body.appendChild(toggleButton);
        }
        
        return toggleButton;
    }
    
    // Initialize theme toggle
    const themeToggle = createThemeToggle();
    const html = document.documentElement;
    
    // Get saved theme or default to light
    function getSavedTheme() {
        try {
            return localStorage.getItem('theme') || 'light';
        } catch (e) {
            console.warn('localStorage not available, using default theme');
            return 'light';
        }
    }
    
    // Save theme preference
    function saveTheme(theme) {
        try {
            localStorage.setItem('theme', theme);
        } catch (e) {
            console.warn('Could not save theme preference');
        }
    }
    
    // Apply theme
    function applyTheme(theme) {
        if (theme === 'dark') {
            html.setAttribute('data-theme', 'dark');
            themeToggle.innerHTML = '☀️';
            themeToggle.setAttribute('aria-label', 'Switch to light mode');
            themeToggle.setAttribute('title', 'Switch to light theme');
        } else {
            html.removeAttribute('data-theme');
            themeToggle.innerHTML = '🌙';
            themeToggle.setAttribute('aria-label', 'Switch to dark mode');
            themeToggle.setAttribute('title', 'Switch to dark theme');
        }
    }
    
    // Initialize with saved theme
    const currentTheme = getSavedTheme();
    applyTheme(currentTheme);
    
    // Add click event listener
    themeToggle.addEventListener('click', function() {
        const isCurrentlyDark = html.getAttribute('data-theme') === 'dark';
        const newTheme = isCurrentlyDark ? 'light' : 'dark';
        
        applyTheme(newTheme);
        saveTheme(newTheme);
        
        console.log('Theme switched to:', newTheme);
    });
    
    // Handle system preference changes (optional enhancement)
    if (window.matchMedia) {
        const mediaQuery = window.matchMedia('(prefers-color-scheme: dark)');
        
        // Only apply system preference if no saved preference exists
        if (!localStorage.getItem('theme')) {
            applyTheme(mediaQuery.matches ? 'dark' : 'light');
        }
        
        // Listen for system preference changes
        mediaQuery.addEventListener('change', function(e) {
            // Only apply if no saved preference exists
            if (!localStorage.getItem('theme')) {
                applyTheme(e.matches ? 'dark' : 'light');
            }
        });
    }
    
    console.log('Theme toggle initialized with theme:', currentTheme);
});