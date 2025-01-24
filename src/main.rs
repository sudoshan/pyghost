use std::process::ExitCode;
use std::{env, fs};

fn color_folder(home: &str) -> String {
    match env::var("XDG_CACHE_HOME") {
        Ok(cache_dir) => format!("{cache_dir}/wal/colors"),
        Err(_) => format!("{home}/.cache/wal/colors"),
    }
}

fn config_folder(home: &str) -> String {
    match env::var("XDG_CONFIG_HOME") {
        Ok(conf_dir) => format!("{conf_dir}/ghostty/config"),
        Err(_) => format!("{home}/.config/ghostty/config"),
    }
}

fn read_options(conf: &str) -> Vec<&str> {
    conf.lines()
        .filter(|line| {
            line.split_once('=')
                .map(|(key, _)| {
                    !matches!(
                        key.trim(),
                        "theme" | "palette" | "background" | "foreground",
                    )
                })
                .unwrap_or(true)
        })
        .collect()
}

fn get_new_config(colors: Vec<&str>, old_config: Vec<&str>) -> String {
    old_config.join("\n")
        + "\n"
        + &colors
            .iter()
            .enumerate()
            .map(|(i, c)| format!("palette = {i}={c}"))
            .fold(String::new(), |mut acc: String, s: String| -> String {
                acc.reserve(s.len() + 1);
                acc.push_str(&s);
                acc.push('\n');
                acc
            })
        + &format!(
            "background = {}\n\
             foreground = {}\n",
            colors[0],
            colors[colors.len() - 1]
        )
}

fn main() -> ExitCode {
    let Ok(home) = env::var("HOME") else {
        eprintln!("error, home");
        return ExitCode::FAILURE;
    };
    let color_dir = color_folder(&home);
    let conf_dir = config_folder(&home);

    let Ok(colors) = fs::read_to_string(&color_dir) else {
        eprintln!("error, opening {color_dir}");
        return ExitCode::FAILURE;
    };
    let colors: Vec<&str> = colors.lines().collect();

    let Ok(conf) = fs::read_to_string(&conf_dir) else {
        eprintln!("error, opening {conf_dir}");
        return ExitCode::FAILURE;
    };

    let options = read_options(&conf);

    if let Err(_) = fs::write(&conf_dir, get_new_config(colors, options)) {
        eprintln!("error, writing {conf_dir}");
        return ExitCode::FAILURE;
    };

    ExitCode::SUCCESS
}
