# phantom

# this project is incomplete, meaning its progress is still on-going.

phantom is an implementation of pywal colors in ghostty. unlike pywal, instead of changing tty colors, phantom directly changes the theming on ghostty as per your currrent pywal colors.

phantom most likely wont work on your computer initially and you will have to change the directory to your pywal colors yourself in order to get it to read your pywal colors.

# if there are color palettes defined in your ghostty config file initially, no other settings after the definition of those palettes in your config file will be read, as they will be removed in order for phantom to add the new pywal generated colors to your config.
