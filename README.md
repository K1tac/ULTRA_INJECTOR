# ULTRA\_INJECTOR

**ULTRA\_INJECTOR** is an open-source injector designed specifically for **ULTRAKILL.exe**.
It lets you inject *any* DLL into the game — cheats, mods, debug tools, whatever you want.

---

## 🛠️ What it does

* Automatically finds the running `ULTRAKILL.exe` process
* Injects a DLL of your choice into it
* Designed to be used from a WPF app, console tool, or any external C#/C++ project

## ❗ Notes

* Target is hardcoded to `ULTRAKILL.exe` (literally why it's called ULTRA_INJECTOR). If you want to change it, edit `TARGET_PROCESS` in the source.
* You must have the target process running **before** calling `Inject()`.

---

## 🤝 Contributing

If there's a bug or you want to improve something, submit a pull request or open an issue.
(Or just use ChatGPT.)

---

## 🧪 For the no attention span skids:

* ✅ One single DLL
* ✅ No EXE needed
* ✅ Injects anything
* ✅ Easy to embed in tools
* ✅ C#, WPF, or even C++ friendly

(I'm sorry for calling you a skid.)

---

## 📦 License

Do whatever, Copy it. Sell it. Just don't DM me asking how to compile a DLL.
(But note that this is open sourced so you prolly ain't gonna get sales)
