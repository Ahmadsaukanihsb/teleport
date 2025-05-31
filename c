local targetFunc = nil

-- Temukan ulang fungsi tersebut
for _, f in pairs(getgc(true)) do
    if typeof(f) == "function" and islclosure(f) then
        local constants = debug.getconstants(f)
        for _, c in pairs(constants) do
            if tostring(c):lower():find("acceptpetgift") then
                targetFunc = f
                break
            end
        end
    end
    if targetFunc then break end
end

if targetFunc then
    print("✅ Function ditemukan: ", targetFunc)

    print("🔍 Constants:")
    for i, v in ipairs(debug.getconstants(targetFunc)) do
        print(i, v)
    end

    print("📦 Upvalues:")
    for i = 1, debug.getupvalue(targetFunc, math.huge) do
        local name, val = debug.getupvalue(targetFunc, i)
        print(i, name, val)
    end
else
    warn("❌ Function tidak ditemukan.")
end
