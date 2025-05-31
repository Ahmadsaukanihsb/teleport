local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")

local localPlayer = Players.LocalPlayer
while not localPlayer do task.wait(1) localPlayer = Players.LocalPlayer end

-- ===============================================
-- FUNGSI UTAMA UNTUK MENDETEKSI SEMUA REMOTE EVENT/FUNCTION
-- ===============================================
local function dumpAllRemotes()
    print("\n[SCAN REMOTE] Mencari semua RemoteEvent/RemoteFunction di game...")
    
    -- Cari di ReplicatedStorage dulu
    for _, remote in ipairs(ReplicatedStorage:GetDescendants()) do
        if remote:IsA("RemoteEvent") or remote:IsA("RemoteFunction") then
            print("[REMOTE FOUND]", remote:GetFullName())
        end
    end
    
    -- Cari di tempat lain yang mungkin
    local importantLocations = {
        game:GetService("ServerScriptService"),
        game:GetService("ServerStorage"),
        game:GetService("Workspace"),
        localPlayer:WaitForChild("PlayerScripts")
    }
    
    for _, location in ipairs(importantLocations) do
        for _, remote in ipairs(location:GetDescendants()) do
            if remote:IsA("RemoteEvent") or remote:IsA("RemoteFunction") then
                print("[REMOTE FOUND]", remote:GetFullName())
            end
        end
    end
end

-- ===============================================
-- OPSI 1: AUTO-CLAIM JIKA REMOTE DIKETAHUI
-- ===============================================
local function tryClaimGifts()
    -- Ganti "GiftRemote" dengan nama remote yang benar
    local giftRemote = ReplicatedStorage:FindFirstChild("GiftRemote") 
        or ReplicatedStorage:FindFirstChild("GiftEvent")
        or ReplicatedStorage:FindFirstChild("ClaimGifts")
    
    if giftRemote then
        pcall(function()
            if giftRemote:IsA("RemoteEvent") then
                giftRemote:FireServer("ClaimAll")
            else
                giftRemote:InvokeServer("ClaimAll")
            end
            print("[SUCCESS] Gift berhasil di-claim!")
        end)
    else
        warn("[ERROR] Tidak menemukan gift remote!")
        dumpAllRemotes() -- Scan semua remote yang ada
    end
end

-- ===============================================
-- OPSI 2: CLAIM VIA UI JIKA REMOTE TIDAK DITEMUKAN
-- ===============================================
local function claimViaUI()
    local gui = localPlayer:WaitForChild("PlayerGui")
    local giftButton = gui:FindFirstChild("GiftButton", true) -- Cari tombol gift
    
    if giftButton and giftButton:IsA("TextButton") then
        giftButton:FireServer("MouseButton1Click")
        print("[SUCCESS] Gift di-claim via UI!")
    else
        warn("[ERROR] Tidak menemukan tombol gift di GUI!")
    end
end

-- ===============================================
-- EXECUTE
-- ===============================================
while task.wait(5) do
    tryClaimGifts()
    claimViaUI() -- Backup plan jika remote tidak ditemukan
end
