local Players = game:GetService("Players")
local TeleportService = game:GetService("TeleportService")
local HttpService = game:GetService("HttpService")
local TextChatService = game:GetService("TextChatService")

-- Konfigurasi
local API_URL = "https://api-beta-mocha-45.vercel.app/api/getInstanceId"
local PLACE_ID = 126884695634066
local CHAT_MESSAGE = "Pesan otomatis dari script!"
local MAX_CHAT_PER_INSTANCE = 3
local CHECK_INTERVAL = 5  -- Detik
local TELEPORT_COOLDOWN = 15  -- Detik

-- Variabel sistem
local lastTeleportTime = 0
local instanceChatCounts = {}
local isChatEnabled = true

-- Fungsi HTTP request dengan retry
local function getNewInstanceId()
    for attempt = 1, 3 do
        local success, response = pcall(function()
            return HttpService:RequestAsync({
                Url = API_URL,
                Method = "GET",
                Headers = {
                    ["Content-Type"] = "application/json"
                }
            })
        end)

        if success and response.Success then
            local data = HttpService:JSONDecode(response.Body)
            if data and data.instanceId then
                return data.instanceId
            end
        end
        task.wait(2 ^ attempt)  -- Exponential backoff
    end
    return nil
end

-- Fungsi kirim chat dengan proteksi
local function sendChat()
    if not isChatEnabled then return false end
    
    -- Inisialisasi counter
    if not instanceChatCounts[game.JobId] then
        instanceChatCounts[game.JobId] = 0
    end

    -- Cek batas chat
    if instanceChatCounts[game.JobId] >= MAX_CHAT_PER_INSTANCE then
        print("🚫 Batas chat tercapai ("..MAX_CHAT_PER_INSTANCE.."x/instance)")
        return false
    end

    -- Cek player lain di radius 20 studs
    local localPlayer = Players.LocalPlayer
    if localPlayer and localPlayer.Character then
        local humanoidRootPart = localPlayer.Character:FindFirstChild("HumanoidRootPart")
        if humanoidRootPart then
            local localPos = humanoidRootPart.Position
            for _, player in ipairs(Players:GetPlayers()) do
                if player ~= localPlayer and player.Character then
                    local targetPart = player.Character:FindFirstChild("HumanoidRootPart")
                    if targetPart and (targetPart.Position - localPos).Magnitude <= 20 then
                        print("👥 Player terdeteksi di dekat - skip chat")
                        return false
                    end
                end
            end
        end
    end

    -- Kirim chat
    local success = pcall(function()
        TextChatService.TextChannels.RBXGeneral:SendAsync(CHAT_MESSAGE)
        instanceChatCounts[game.JobId] += 1
        print("💬 Chat terkirim ("..instanceChatCounts[game.JobId].."/"..MAX_CHAT_PER_INSTANCE..")")
    end)

    return success
end

-- Fungsi teleport
local function teleportToNewInstance()
    if os.time() - lastTeleportTime < TELEPORT_COOLDOWN then
        print("⏳ Cooldown teleport:", TELEPORT_COOLDOWN - (os.time() - lastTeleportTime), "detik")
        return false
    end

    local instanceId = getNewInstanceId()
    if not instanceId then
        warn("Gagal mendapatkan instance ID")
        return false
    end

    if instanceId == game.JobId then
        print("✅ Sudah di instance terbaru")
        return false
    end

    print("🚀 Teleport ke instance baru:", instanceId)
    lastTeleportTime = os.time()

    -- Reset counter chat instance lama
    instanceChatCounts[game.JobId] = nil

    pcall(function()
        TeleportService:TeleportToPlaceInstance(PLACE_ID, instanceId, Players.LocalPlayer)
    end)
    return true
end

-- Fungsi utama
local function main()
    while task.wait(CHECK_INTERVAL) do
        print("\n=== Sistem Aktif ===")
        print("Instance saat ini:", game.JobId)
        
        -- 1. Coba teleport dulu jika diperlukan
        if not teleportToNewInstance() then
            -- 2. Jika tidak teleport, kirim chat
            sendChat()
        end
    end
end

-- Jalankan script
local success, err = pcall(main)
if not success then
    warn("❌ Script error:", err)
    -- Auto-restart setelah 10 detik
    task.wait(10)
    pcall(main)
end
