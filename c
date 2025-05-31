--[[
    AUTO ACCEPT GIFT SCRIPT (ROBLOX)
    Fix utama untuk error 'attempt to call nil value'
    By: ChatDev
--]]

-- 1. INISIALISASI AMAN
local function safeGetService(serviceName)
    local service
    pcall(function()
        service = game:GetService(serviceName)
    end)
    return service
end

local Players = safeGetService("Players") or game:GetService("Players")
local UserInputService = safeGetService("UserInputService")
local RunService = safeGetService("RunService")

if not (Players and UserInputService and RunService) then
    error("Essential services not found")
end

-- 2. HANDLING PLAYER
local localPlayer
local maxAttempts = 10
local attempts = 0

repeat
    localPlayer = Players.LocalPlayer
    attempts = attempts + 1
    task.wait(1)
until localPlayer or attempts >= maxAttempts

if not localPlayer then
    error("Failed to get LocalPlayer after "..maxAttempts.." attempts")
end

print("Player initialized:", localPlayer.Name)

-- 3. TOMBOL DETECTION
local function getAcceptButton()
    -- Versi aman untuk FindFirstChild
    local function safeFind(parent, name)
        return pcall(function() return parent:FindFirstChild(name, true) end)
    end

    -- Cari di 3 lokasi potensial
    local searchLocations = {
        {path = {"PlayerGui", "FriendInvites", "Insertio!Point", "FriendInviteTemplate", "Holder", "Frame", "Accept"}, reason = "Path spesifik"},
        {path = {"PlayerGui", "GiftNotification", "AcceptButton"}, reason = "Struktur umum"},
        {path = {"PlayerGui", "MainGUI", "Gifts", "Accept"}, reason = "Alternatif"}
    }

    for _, location in ipairs(searchLocations) do
        local current = localPlayer
        local found = true
        
        for _, childName in ipairs(location.path) do
            local success, child = safeFind(current, childName)
            if not success or not child then
                found = false
                break
            end
            current = child
        end

        if found then
            print("Tombol ditemukan via:", location.reason)
            return current
        end
    end

    -- Fallback: Cari berdasarkan teks
    local gui = localPlayer:FindFirstChildOfClass("PlayerGui")
    if gui then
        for _, element in ipairs(gui:GetDescendants()) do
            if (element:IsA("TextButton") or element:IsA("ImageButton")) then
                if string.find(tostring(element.Text):lower(), "accept") or 
                   string.find(tostring(element.Name):lower(), "accept") then
                    print("Tombol ditemukan via fallback text")
                    return element
                end
            end
        end
    end

    return nil
end

-- 4. KLIK TOMBOL MULTI-METODE
local function clickButton(button)
    local clickMethods = {
        {name = "Activated", func = function()
            for _, conn in ipairs(getconnections(button.Activated)) do
                conn:Fire()
            end
        end},
        {name = "MouseButton1Click", func = function()
            button.MouseButton1Click:Fire()
        end},
        {name = "VirtualClick", func = function()
            local pos = button.AbsolutePosition + button.AbsoluteSize/2
            UserInputService:SendMouseButtonEvent(pos.X, pos.Y, 0, true, game)
            task.wait(0.05)
            UserInputService:SendMouseButtonEvent(pos.X, pos.Y, 0, false, game)
        end}
    }

    for _, method in ipairs(clickMethods) do
        local success, err = pcall(method.func)
        if success then
            print("Klik berhasil via", method.name)
            return true
        else
            print("Gagal klik via", method.name..":", err)
        end
    end

    return false
end

-- 5. MAIN SYSTEM
local function main()
    local button = getAcceptButton()
    if button then
        print("Mencoba klik tombol di:", button:GetFullName())
        clickButton(button)
    else
        print("Tombol belum ditemukan")
    end
end

-- 6. EXECUTION LOOP
local heartbeatConnection
heartbeatConnection = RunService.Heartbeat:Connect(function()
    local success, err = pcall(main)
    if not success then
        warn("Runtime error:", err)
        heartbeatConnection:Disconnect()
        task.wait(2)
        heartbeatConnection = RunService.Heartbeat:Connect(function()
            pcall(main)
        end)
    end
end)

-- Auto-cleanup
game:GetService("StarterGui"):SetCore("ResetButtonCallback", function()
    heartbeatConnection:Disconnect()
end)

print("Script berjalan dengan aman!")
