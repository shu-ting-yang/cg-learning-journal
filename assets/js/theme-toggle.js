// Theme toggle functionality
(function() {
  // Get current theme from localStorage or default to 'default'
  const currentTheme = localStorage.getItem('theme') || 'default';
  
  // Apply theme on page load
  document.documentElement.setAttribute('data-theme', currentTheme);
  
  // Update toggle button state
  function updateToggleButton() {
    const toggleButton = document.getElementById('theme-toggle');
    if (toggleButton) {
      const isDark = document.documentElement.getAttribute('data-theme') === 'dark';
      toggleButton.textContent = isDark ? '☀️' : '🌙';
      toggleButton.setAttribute('aria-label', isDark ? 'Switch to light theme' : 'Switch to dark theme');
    }
  }
  
  // Toggle theme function
  function toggleTheme() {
    const currentTheme = document.documentElement.getAttribute('data-theme');
    const newTheme = currentTheme === 'dark' ? 'default' : 'dark';
    
    document.documentElement.setAttribute('data-theme', newTheme);
    localStorage.setItem('theme', newTheme);
    updateToggleButton();
  }
  
  // Initialize when DOM is loaded
  document.addEventListener('DOMContentLoaded', function() {
    updateToggleButton();
    
    // Add click listener to toggle button
    const toggleButton = document.getElementById('theme-toggle');
    if (toggleButton) {
      toggleButton.addEventListener('click', toggleTheme);
    }
  });
})();