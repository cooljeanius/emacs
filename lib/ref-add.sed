/^# Packages using this file: / {
  s/# Packages using this file://
  ta
  :a
  s/ emacs / emacs /
  tb
  s/ $/ emacs /
  :b
  s/^/# Packages using this file:/
}
