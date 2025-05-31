for _, f in pairs(getgc(true)) do
    if typeof(f) == "function" and islclosure(f) then
        local constants = debug.getconstants(f)
        for _, v in pairs(constants) do
            if tostring(v):lower():find("acceptpetgift") then
                print("📌 Function ditemukan:")
                print(f)
                print("📦 Constants:", table.concat(constants, ", "))
            end
        end
    end
end
