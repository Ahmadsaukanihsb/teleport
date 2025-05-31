local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local TextChatService = game:GetService("TextChatService")

local localPlayer = Players.LocalPlayer
local GIFT_COOLDOWN = 3
local lastGiftCheck = 0

-- Fungsi untuk menerima gift dari friend
local function acceptFriendGifts()
    local friendGiftEvent = ReplicatedStorage:FindFirstChild("FriendGiftEvent")
    if not friendGiftEvent then return false end

    -- Format parameter berdasarkan debug log
    local success = pcall(function()
        -- Coba dengan parameter berbeda
        friendGiftEvent:FireServer("accept_all")
        friendGiftEvent:FireServer(localPlayer.UserId)
        friendGiftEvent:FireServer({
            player = localPlayer,
            action = "accept_all",
            timestamp = os.time()
        })
        return true
    end)
    
    return success
end

-- Fungsi untuk menerima pet gifts
local function acceptPetGifts()
    local petGiftService = ReplicatedStorage:FindFirstChild("PetGiftingService")
    if not petGiftService then return false end

    local success = pcall(function()
        -- Format khusus untuk pet gifts
        petGiftService:FireServer("collect_all")
        petGiftService:FireServer({
            UserId = localPlayer.UserId,
            Action = "ClaimAll"
        })
        return true
    end)
    
    return success
end

-- Fungsi utama
local function collectAllGifts()
    if os.time() - lastGiftCheck < GIFT_COOLDOWN then return end
    lastGiftCheck = os.time()

    print("\n[System] Memulai proses penerimaan gift...")
    
    -- 1. Terima friend gifts
    if acceptFriendGifts() then
        print("[Success] Friend gifts berhasil diterima")
    else
        print("[Warning] Gagal menerima friend gifts")
    end

    -- 2. Terima pet gifts
    if acceptPetGifts() then
        print("[Success] Pet gifts berhasil diterima")
    else
        print("[Warning] Gagal menerima pet gifts")
    end

    -- 3. Aktivitas tambahan untuk hindari AFK
    if math.random(1, 10) == 1 then
        pcall(function()
            TextChatService.TextChannels.RBXGeneral:SendAsync("Auto-gift collector aktif")
        end)
    end
end

-- Auto-execute
while task.wait(GIFT_COOLDOWN) do
    pcall(collectAllGifts)
end

print("Script gift auto-accept berhasil dijalankan!")
