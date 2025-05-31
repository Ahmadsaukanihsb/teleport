-- [1] Cari function yang memanggil AcceptPetGift
local targetFunc = nil

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

-- [2] Jika ditemukan, tampilkan isi constants dan upvalues
if targetFunc then
    print("✅ Function ditemukan: ", targetFunc)

    print("🔍 Constants:")
    for i, v in ipairs(debug.getconstants(targetFunc)) do
        print(i, v)
    end

    print("📦 Upvalues:")
    for i = 1, debug.getinfo(targetFunc).nups do
        local name, val = debug.getupvalue(targetFunc, i)
        print(i, name, val)
    end

    -- [3] Hook function untuk melihat argumen yang dikirim saat AcceptPetGift dipanggil
    local oldFunc = targetFunc
    hookfunction(oldFunc, function(...)
        local args = {...}
        print("🎁 AcceptPetGift dipanggil dengan argumen:")
        for i, v in ipairs(args) do
            print(i, v)
        end
        return oldFunc(...)
    end)

else
    warn("❌ Function AcceptPetGift tidak ditemukan.")
end
