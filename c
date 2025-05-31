local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local TextChatService = game:GetService("TextChatService")

-- Enhanced configuration
local GIFT_COOLDOWN = 3
local lastGiftCheck = 0
local DEBUG_MODE = true

local function debugPrint(...)
    if DEBUG_MODE then
        print("[Gift Debug]", ...)
    end
end

local function acceptGiftsAutomatically()
    if os.time() - lastGiftCheck < GIFT_COOLDOWN then return false end
    lastGiftCheck = os.time()

    -- Method 1: Advanced RemoteEvent detection with parameter analysis
    debugPrint("Scanning for gift-related RemoteEvents...")
    for _, remote in ipairs(ReplicatedStorage:GetDescendants()) do
        if remote:IsA("RemoteEvent") then
            local nameLower = string.lower(remote.Name)
            if string.find(nameLower, "gift") or string.find(nameLower, "reward") or string.find(nameLower, "receive") then
                debugPrint("Found potential gift RemoteEvent:", remote.Name)
                
                -- Try various parameter combinations including caster data
                local testParams = {
                    {},
                    {"accept"},
                    {true},
                    {Players.LocalPlayer},
                    {"accept_all"},
                    {Players.LocalPlayer.UserId},
                    {Players.LocalPlayer, "accept"},
                    {userId = Players.LocalPlayer.UserId, action = "accept"}
                }

                for _, params in ipairs(testParams) do
                    pcall(function()
                        remote:FireServer(unpack(params))
                        debugPrint("Fired", remote.Name, "with params:", table.concat(params, ", "))
                    end)
                    task.wait(0.2)
                end
            end
        end
    end

    -- Method 2: Handle missing caster data specifically
    debugPrint("Attempting to fix missing caster data...")
    pcall(function()
        local giftEvent = ReplicatedStorage:FindFirstChild("GiftEvent") or
                         ReplicatedStorage:FindFirstChild("GiftRemote")
        
        if giftEvent then
            -- Simulate caster data structure
            local fakeCaster = {
                UserId = Players.LocalPlayer.UserId,
                Name = Players.LocalPlayer.Name,
                DisplayName = Players.LocalPlayer.DisplayName
            }
            
            giftEvent:FireServer({
                caster = fakeCaster,
                recipient = Players.LocalPlayer,
                giftData = {type = "auto_collected"}
            })
            debugPrint("Sent simulated caster data")
            return true
        end
    end)

    -- Method 3: Direct inventory addition fallback
    debugPrint("Attempting direct inventory addition...")
    pcall(function()
        local inventoryService = game:GetService("InventoryService")
        if inventoryService then
            inventoryService:AddItem(Players.LocalPlayer, "GiftBox")
            debugPrint("Attempted direct inventory add")
        end
    end)

    debugPrint("All methods attempted")
    return false
end

-- Main execution with improved error handling
while true do
    local success, err = pcall(acceptGiftsAutomatically)
    if not success then
        debugPrint("Error in main loop:", err)
    end
    task.wait(GIFT_COOLDOWN)
end
